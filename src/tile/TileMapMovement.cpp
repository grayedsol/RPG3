/**
 * @file TileMapMovement.cpp
 * @author Grayedsol (grayedsol@gmail.com)
 * @copyright Copyright (c) 2024
 */
#include "TileMapMovement.hpp"
#include "../scenes/TileMapScene.hpp"

TileMapMovement::TileMapMovement(TileMapScene* scene) :
	scene(scene),
	positions(scene->getECS().getComponent<Position2>()),
	velocities(scene->getECS().getComponent<Velocity2>()),
	actors(scene->getECS().getComponent<Actor>()),
	sprites(scene->getECS().getComponent<ActorSprite>()),
	players(scene->getECSReadOnly().getComponentReadOnly<Player>()) {
}

/**
 * @details
 * The movement direction is calculated by adding the vertical and
 * horizontal inputs together. For example, since Actor::Direction::Down
 * is 2 and Actor::Direction::Right is 6, add 2 and 6 to get 8, which is
 * the value of Actor::Direction::RightDown.
 * If both up and down are input, they cancel each other out; the same
 * goes for left and right.
 * 
 * The bit shifting is an over-engineered branchless version of this code:
 * @code{.cpp}
 * 	if (scene->isPressing(GCmd::MapDown) && !scene->isPressing(GCmd::MapUp)) {
 * 		direction += static_cast<uint8_t>(Actor::Direction::Down);
 * 	}
 * 	else if (scene->isPressing(GCmd::MapUp) && !scene->isPressing(GCmd::MapDown)) {
 * 		direction += static_cast<uint8_t>(Actor::Direction::Up);
 * 	}
 * 
 * 	if (scene->isPressing(GCmd::MapLeft) && !scene->isPressing(GCmd::MapRight)) {
 * 		direction += static_cast<uint8_t>(Actor::Direction::Left);
 * 	}
 * 	else if (scene->isPressing(GCmd::MapRight) && !scene->isPressing(GCmd::MapLeft)) {
 * 		direction += static_cast<uint8_t>(Actor::Direction::Right);
 * 	}
 * @endcode
 */
void TileMapMovement::process(double delta) {
	for (auto e : players) {
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

		Tileset& tileset = scene->getTileEntityMap().tilesets[sprites.get(e).tileset];
		positions.get(e)[0] += (scene->isPressing(GCmd::MapRight) - scene->isPressing(GCmd::MapLeft)) * actors.get(e).speed * delta;
		positions.get(e)[1] += (scene->isPressing(GCmd::MapDown) - scene->isPressing(GCmd::MapUp)) * actors.get(e).speed * delta;

		if (direction) {
			sprites.get(e).index = direction;
			tileset.processAnimations(delta);
			actors.get(e).direction = static_cast<Actor::Direction>(direction);
		}
		else {
			tileset.textureIdx[actors.get(e).direction] = actors.get(e).direction;
		}
	}
}