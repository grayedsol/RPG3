/**
 * @file TileMapMovement.cpp
 * @author Grayedsol (grayedsol@gmail.com)
 * @copyright Copyright (c) 2024
 */
#include "TileMapMovement.hpp"
#include "../scenes/TileMapScene.hpp"
#include "QuadTree.hpp"

static const float INV_SQRT_TWO = 0.7071f;
static const unsigned MAX_COLLISION_RESOLUTION_ATTEMPTS = 6;

static Velocity2 dirVecs[Tile::Direction::DirectionSize] = {
	Velocity2{0,0},
	Velocity2{0,1},
	Velocity2{0,-1},
	Velocity2{-1,0},
	Velocity2{-INV_SQRT_TWO,INV_SQRT_TWO},
	Velocity2{-INV_SQRT_TWO,-INV_SQRT_TWO},
	Velocity2{1,0},
	Velocity2{INV_SQRT_TWO,INV_SQRT_TWO},
	Velocity2{INV_SQRT_TWO,-INV_SQRT_TWO}
};

static Tile::Direction vecDirs[Tile::Direction::DirectionSize] {
	Tile::Direction::LeftUp,
	Tile::Direction::Left,
	Tile::Direction::LeftDown,
	Tile::Direction::Up,
	Tile::Direction::DirectionNone,
	Tile::Direction::Down,
	Tile::Direction::RightUp,
	Tile::Direction::Right,
	Tile::Direction::RightDown
};

static Tile::Direction vecToDir(Velocity2 vec) {
	return vecDirs[(int)((vec[0]+1)*3+vec[1]+1)];
}

void Tile::MapMovement::glide(double delta, Velocity2 prevVelocity, ECS::entity e) {
	for (int i = 0; i < 2; i++) {
		/* Proceed only if the actor was moving in this coordinate last frame but not this one */
		if (!prevVelocity[i] || velocities->get(e)[i]) { continue; }
		/* Calculate this coordinate's remainder after flooring */
		float rmndr = fabsf(positions->get(e)[i] - floorf(positions->get(e)[i]));
		/* If it's zero, it's not mid-pixel, so we don't need to glide */
		if (!rmndr) { continue; }
		/* Try an incremental move, and floor it */
		rmndr = floorf(rmndr + prevVelocity[i] * (actors->get(e).speed * (1 + actors->get(e).sprinting) * delta));
		/* If it did not floor to 0, it escaped the range [0, 1), so it crossed over a pixel */
		if (rmndr) {
			/* Snap to the pixel it crossed, using either floor or ceil */
			positions->get(e)[i] = prevVelocity[i] < 0 ? floorf(positions->get(e)[i]) : ceilf(positions->get(e)[i]);
		}
		else {
			/* If it did floor to 0, it's still mid-pixel, so glide it */
			velocities->get(e)[i] = prevVelocity[i];
		}		
	}
}

static Velocity2 AABBMTV(const Hitbox& lhs, const Hitbox& rhs) {
	Velocity2 returnVec{ 0, 0 };

	float left = rhs.x - (lhs.x + lhs.w);
	float right = (rhs.x + rhs.w) - lhs.x;
	float bottom = rhs.y - (lhs.y + lhs.h);
	float top = (rhs.y + rhs.h) - lhs.y;

	returnVec.x = fabsf(left) > right ? right : left;
	returnVec.y = fabsf(bottom) > top ? top : bottom;

	if (fabsf(returnVec.x) >= fabsf(returnVec.y)) { returnVec.x = 0; }
	else { returnVec.y = 0; }

	return returnVec;
}

Hitbox Tile::MapMovement::handleEntityCollisions(Hitbox box, ECS::entity e, int layer, unsigned attempts) {
	std::vector<Hitbox> eCollisions;
	scene->getQuadTrees().at(layer).query(box, e, eCollisions);
	if (eCollisions.empty()) { return box; }
	*((Position2*)&box) += AABBMTV(box, eCollisions.back());

	if (attempts >= MAX_COLLISION_RESOLUTION_ATTEMPTS) { return box; }
	return handleEntityCollisions(box, e, layer, attempts + 1);
}

Hitbox Tile::MapMovement::handleTileCollisions(Hitbox box, int layer) {
	SDL_FRect* rect = reinterpret_cast<SDL_FRect*>(&box);
	std::vector<SDL_FRect> collisions = scene->queryTileCollisions(*rect, layer);
	if (collisions.empty()) { return box; }
	SDL_FRect f = collisions.back();
	*((Position2*)&box) += AABBMTV(box, *(Hitbox*)&f);
	return handleTileCollisions(box, layer);
}

void Tile::MapMovement::handleSoftEntityCollisions(Hitbox box, ECS::entity e, int layer) {
	std::vector<entity> eCollisions;
	scene->getSoftQuadTrees().at(layer).query(box, e, eCollisions);
	for (auto e : eCollisions) {
		if (collisionInteractions->contains(e)) {
			MapCollisionInteraction& interaction = collisionInteractions->get(e);
			interaction.beingPressed = true;
			if (!interaction.active) { scene->executeCommand(interaction.command); }
			if (interaction.mode == MapCollisionInteraction::Mode::Fleeting) {
				scene->getECS().freeEntity(e);
			}
			else if (interaction.mode == MapCollisionInteraction::Mode::PressurePlate) {
				interaction.active = true;
			}
		}
	}
}

Tile::MapMovement::MapMovement(MapScene *scene) :
	scene(scene),
	positions(&scene->getECS().getComponent<Position2>()),
	velocities(&scene->getECS().getComponent<Velocity2>()),
	hitboxes(&scene->getECS().getComponent<Hitbox>()),
	mapEntities(&scene->getECS().getComponentReadOnly<MapEntity>()),
	actors(&scene->getECS().getComponent<Actor>()),
	sprites(&scene->getECS().getComponent<ActorSprite>()),
	players(&scene->getECSReadOnly().getComponentReadOnly<Player>()),
	collisionInteractions(&scene->getECS().getComponent<MapCollisionInteraction>()) {
}

/**
 * @details
 * Updates the actor's velocity based on its direction and whether it is moving or not.
 * 
 * Also glides the actor when it stops moving in a direction, so that it will align
 * to a pixel before it stops moving in that direction.
 */
void Tile::MapMovement::process(double delta) {
	for (auto e : *actors) {
		/* Save the previous velocity for when we check for gliding */
		Velocity2 prevVelocity = velocities->get(e);
		
		/* Update the velocity based on direction. If it's not moving, use the 0 vector */
		velocities->get(e) = dirVecs[actors->get(e).movingDirection ? actors->get(e).direction : 0];
		
		/* Try gliding */
		glide(delta, prevVelocity, e);

		unsigned layer = mapEntities->get(e).layer;
		if (hitboxes->contains(e)) {
			Hitbox box = hitboxes->get(e);
			Hitbox oldBox = box;
			Position2* pos = reinterpret_cast<Position2*>(&box);
			*pos = positions->get(e);
			*pos += velocities->get(e) * actors->get(e).speed * (1 + actors->get(e).sprinting) * delta;

			box = handleEntityCollisions(box, e, layer);
			box = handleTileCollisions(box, layer);

			positions->get(e) = *pos;
			hitboxes->get(e) = box;

			handleSoftEntityCollisions(box, e, layer);

			/**
			 * We update the quadtree here to prevent jittering that
			 * would occur if only previous frame collision data was used.
			 * However, this may produce collision inaccuracies that
			 * last for one frame, especially for big/teleport movements.
			 */
			scene->updateQuadTree(oldBox, box, e, layer);

			EntityMap::sortLayer(&scene->getTileEntityMap(), layer);
		}
		else {
			positions->get(e) += velocities->get(e) * actors->get(e).speed * (1 + actors->get(e).sprinting) * delta;
			EntityMap::sortLayer(&scene->getTileEntityMap(), layer);
		}
	}
	for (auto& interaction : collisionInteractions->value) {
		if (interaction.beingPressed == false) {
			interaction.active = false;
		}
		interaction.beingPressed = false;
	}
}

void Tile::MapMovement::postProcess() {
	for (auto& actorData : actors->value) {
		actorData.movingDirection = Direction::DirectionNone;
	}
}
