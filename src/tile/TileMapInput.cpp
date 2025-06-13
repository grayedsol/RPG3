/**
 * @file TileMapInput.cpp
 * @author Grayedsol (grayedsol@gmail.com)
 * @copyright Copyright (c) 2025
 */
#include "TileMapInput.hpp"
#include "../scenes/TileMapScene.hpp"

static const float INV_SQRT_TWO = 0.7071f;

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

static Tile::Direction invDirs[Tile::Direction::DirectionSize] {
	Tile::Direction::DirectionNone,
	Tile::Direction::Up,
	Tile::Direction::Down,
	Tile::Direction::Right,
	Tile::Direction::RightUp,
	Tile::Direction::RightDown,
	Tile::Direction::Left,
	Tile::Direction::LeftUp,
	Tile::Direction::LeftDown
};

Tile::MapInput::MapInput(MapScene* scene) :
	scene(scene),
	hitboxes(&scene->getECSReadOnly().getComponentReadOnly<Hitbox>()),
	actors(&scene->getECS().getComponent<Actor>()),
	players(&scene->getECS().getComponent<Player>()),
	actions(&scene->getECSReadOnly().getComponentReadOnly<MapAction>()) {
}

/**
 * @details
 * The movement direction is calculated by adding the vertical and
 * horizontal inputs together. For example, since Tile::Direction::Down
 * is 2 and Tile::Direction::Right is 6, add 2 and 6 to get 8, which is
 * the value of Tile::Direction::RightDown.
 * If both up and down are input, they cancel each other out; the same
 * goes for left and right.
 * 
 * The bit shifting is an over-engineered branchless version of this code:
 * @code{.cpp}
 * 	if (scene->isPressing(GCmd::MapDown) && !scene->isPressing(GCmd::MapUp)) {
 * 		direction += static_cast<uint8_t>(Tile::Direction::Down);
 * 	}
 * 	else if (scene->isPressing(GCmd::MapUp) && !scene->isPressing(GCmd::MapDown)) {
 * 		direction += static_cast<uint8_t>(Tile::Direction::Up);
 * 	}
 * 
 * 	if (scene->isPressing(GCmd::MapLeft) && !scene->isPressing(GCmd::MapRight)) {
 * 		direction += static_cast<uint8_t>(Tile::Direction::Left);
 * 	}
 * 	else if (scene->isPressing(GCmd::MapRight) && !scene->isPressing(GCmd::MapLeft)) {
 * 		direction += static_cast<uint8_t>(Tile::Direction::Right);
 * 	}
 * @endcode
 * @sa Tile::Direction
 */
void Tile::MapInput::process() {
	GRY_Assert(players->size() > 0, "[TileMapInput] There were no players.");
	ECS::entity player = players->getEntity(0);

	/* A bit of bit shifting, see function details documentation. */
	bool r = scene->isPressing(GCmd::MapRight);
	bool lr = scene->isPressing(GCmd::MapLeft) != r;
	bool u = scene->isPressing(GCmd::MapUp);
	bool ud = scene->isPressing(GCmd::MapDown) != u;
	uint8_t direction = lr;
	direction <<= 1;
	direction += lr;
	direction <<= r;
	direction += (ud << u);

	actors->get(player).moving = (bool)direction;
	actors->get(player).sprinting = scene->isPressing(GCmd::MapSprint);
	if (direction) {
		actors->get(player).direction = static_cast<Direction>(direction);
	}

	if (scene->readSingleInput() == GCmd::MapInteract) {
		interact();
	}
}

bool Tile::MapInput::interact() {
	ECS::entity player = players->getEntity(0);
	/* Create a hitbox in front of the player */
	Hitbox searchBox = hitboxes->get(player);
	Velocity2 direction = dirVecs[actors->get(player).direction];
	direction.x *= searchBox.w;
	direction.y *= searchBox.h;
	*(Position2*)&searchBox += direction;

	/* Query for collisions */
	std::vector<ECS::entity> collisions;
	scene->getQuadTrees().at(actors->get(player).layer).query(searchBox, player, collisions);
	if (collisions.empty()) { return false; }

	/* Find the closest collision */
	ECS::entity target = collisions.front();
	Position2 playerPos{ hitboxes->get(player).x, hitboxes->get(player).y };
	Position2 targetPos{ hitboxes->get(target).x, hitboxes->get(target).y };
	float minDistanceSquared = GRY_VecDistanceSq(playerPos, targetPos);
	for (int i = 1; i < collisions.size(); i++) {
		targetPos = Position2{ hitboxes->get(collisions.at(i)).x, hitboxes->get(collisions.at(i)).y };
		if (GRY_VecDistanceSq(playerPos, targetPos) < minDistanceSquared) {
			minDistanceSquared = GRY_VecDistanceSq(playerPos, targetPos);
			target = collisions.at(i);
		}
	}

	/* If they have an action, invoke it */
	if (actions->contains(target)) {
		switch (actions->get(target).type) {
		case MapAction::Speak:
			if (actors->contains(target)) {
				actors->get(target).direction = invDirs[actors->get(player).direction];
				actors->get(target).moving = false;
			}
			players->get(player).speakingTo = target;
			scene->getTileMapSpeak().speak(actions->get(target).id);
			break;
		default:
			GRY_Log("Entity %d had an unknown action.", target);
			break;
		}

		return true;
	}
	return false;
}
