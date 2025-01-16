/**
 * @file TileMapMovement.cpp
 * @author Grayedsol (grayedsol@gmail.com)
 * @copyright Copyright (c) 2024
 */
#include "TileMapMovement.hpp"
#include "../scenes/TileMapScene.hpp"
#include "QuadTree.hpp"

static const float INV_SQRT_TWO = 0.7071f;

static Velocity2 dirVecs[Actor::Direction::SIZE] = {
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

static Actor::Direction vecDirs[Actor::Direction::SIZE] {
	Actor::Direction::LeftUp,
	Actor::Direction::Left,
	Actor::Direction::LeftDown,
	Actor::Direction::Up,
	Actor::Direction::NONE,
	Actor::Direction::Down,
	Actor::Direction::RightUp,
	Actor::Direction::Right,
	Actor::Direction::RightDown
};

static Actor::Direction vecToDir(Velocity2 vec) {
	return vecDirs[(int)((vec[0]+1)*3+vec[1]+1)];
}

void TileMapMovement::glide(double delta, Velocity2 prevVelocity, ECS::entity e) {
	for (int i = 0; i < 2; i++) {
		/* If the actor was moving in this coordinate last frame but not this one */
		if (prevVelocity[i] && !velocities->get(e)[i]) {
			/* Get the decimal part of the coordinate */
			float rmndr = fabsf(positions->get(e)[i] - floorf(positions->get(e)[i]));
			if (rmndr) { /* If it's non-zero, it's mid-pixel and we want to glide */
				/* Try an incremental move, and floor it */
				rmndr = floorf(rmndr + prevVelocity[i] * (actors->get(e).speed * (1 + actors->get(e).sprinting) * delta));
				if (rmndr) { /* If it did not floor to 0, it escaped the range 0-1, so it crossed over a pixel */
					/* Snap to the pixel it crossed, using either floor or ceil */
					positions->get(e)[i] = prevVelocity[i] < 0 ? floorf(positions->get(e)[i]) : ceilf(positions->get(e)[i]);
				}
				else { /* If it did floor to 0, it's still mid-pixel, so glide it */
					velocities->get(e)[i] = prevVelocity[i];
				}
			}
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

Hitbox TileMapMovement::handleEntityCollisions(Hitbox box, ECS::entity e, int layer) {
	std::vector<Hitbox> eCollisions;
	trees.at(layer).query(box, e, eCollisions);
	if (eCollisions.empty()) { return box; }
	*((Position2*)&box) += AABBMTV(box, eCollisions.back());

	return handleEntityCollisions(box, e, layer);
}

Hitbox TileMapMovement::handleTileCollisions(Hitbox box, int layer) {
	SDL_FRect* rect = reinterpret_cast<SDL_FRect*>(&box);
	std::vector<SDL_FRect> collisions = scene->queryCollisions(*rect, layer);
	if (collisions.empty()) { return box; }
	SDL_FRect f = collisions.back();
	*((Position2*)&box) += AABBMTV(box, *(Hitbox*)&f);
	return handleTileCollisions(box, layer);
}

TileMapMovement::TileMapMovement(TileMapScene *scene) :
	scene(scene),
	positions(&scene->getECS().getComponent<Position2>()),
	velocities(&scene->getECS().getComponent<Velocity2>()),
	hitboxes(&scene->getECS().getComponent<Hitbox>()),
	actors(&scene->getECS().getComponent<Actor>()),
	sprites(&scene->getECS().getComponent<ActorSprite>()),
	players(&scene->getECSReadOnly().getComponentReadOnly<Player>()) {
}

/**
 * @details
 * Updates the actor's velocity based on its direction and whether it is moving or not.
 * 
 * Also glides the actor when it stops moving in a direction, so that it will align
 * to a pixel before it stops moving in that direction.
 */
void TileMapMovement::process(double delta) {
	for (auto e : *actors) {
		/* Save the previous velocity for when we check for gliding */
		Velocity2 prevVelocity = velocities->get(e);
		
		/* Update the velocity based on direction. If it's not moving, use the 0 vector */
		velocities->get(e) = dirVecs[actors->get(e).moving ? actors->get(e).direction : 0];

		/* Try gliding */
		glide(delta, prevVelocity, e);
	}

	for (int layer = 0; layer < scene->getTileEntityMap().entityLayers.size(); layer++) {
		for (auto e : scene->getTileEntityMap().entityLayers.at(layer)) {
			if (hitboxes->contains(e)) {
				Hitbox box = hitboxes->get(e);
				Position2* pos = reinterpret_cast<Position2*>(&box);
				*pos = positions->get(e);
				*pos += velocities->get(e) * actors->get(e).speed * (1 + actors->get(e).sprinting) * delta;

				box = handleEntityCollisions(box, e, layer);
				box = handleTileCollisions(box, layer);

				positions->get(e) = *pos;
				hitboxes->get(e) = box;
				TileEntityMap::sortLayer(&scene->getTileEntityMap(), layer);
			}
			else if (velocities->contains(e)) {
				positions->get(e) += velocities->get(e) * actors->get(e).speed * (1 + actors->get(e).sprinting) * delta;
				TileEntityMap::sortLayer(&scene->getTileEntityMap(), layer);
			}
		}
	}
	for (int layer = 0; layer < scene->getTileEntityMap().entityLayers.size(); layer++) {
		trees.at(layer).reset();
		for (auto e : scene->getTileEntityMap().entityLayers.at(layer)) {
			trees.at(layer).insert(hitboxes->get(e), e);
		}
	}
}

void TileMapMovement::init() {
	Hitbox mapSize{0, 0,
		(float)(scene->getTileMap().width * scene->getNormalTileSize()),
		(float)(scene->getTileMap().height * scene->getNormalTileSize())
	};
	for (int layer = 0; layer < scene->getTileEntityMap().entityLayers.size(); layer++) {
		trees.push_back(QuadTree(mapSize));
	}
}
