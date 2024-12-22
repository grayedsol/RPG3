/**
 * @file TileMapMovement.cpp
 * @author Grayedsol (grayedsol@gmail.com)
 * @copyright Copyright (c) 2024
 */
#include "TileMapMovement.hpp"
#include "../scenes/TileMapScene.hpp"

TileMapMovement::TileMapMovement(TileMapScene* scene) :
	scene(scene),
	positions(&scene->getECS().getComponent<Position2>()),
	velocities(&scene->getECS().getComponent<Velocity2>()),
	actors(&scene->getECS().getComponent<Actor>()),
	sprites(&scene->getECS().getComponent<ActorSprite>()),
	players(&scene->getECSReadOnly().getComponentReadOnly<Player>()) {
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
	for (auto e : *players) {
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

		Velocity2 prevVelocity = velocities->get(e);
		/* Bitwise ops are equivalent to Velocity2(right - left, down - up) to make an 8 way direction vector */
		velocities->get(e) = Velocity2((lr & r) - (lr & !r), (ud & !u) - (ud & u));

		if (!lr && prevVelocity[0]) { //if was moving horizontal but no longer
			float rmndr = positions->get(e)[0] - floorf(positions->get(e)[0]); //get the decimal part of the x coordinate
			if (rmndr) { //if it's nonzero (mid pixel)
				rmndr = floorf(rmndr + prevVelocity[0] * (actors->get(e).speed * delta)); //try increment moving and flooring it
				if (rmndr) { //if it did not floor to 0, it escaped the range 0-1, so it crossed over a pixel
					positions->get(e)[0] = rmndr < 0 ? floorf(positions->get(e)[0]) : ceilf(positions->get(e)[0]); //floor or ceil to the pixel it crossed
				}
				else { //move normally and sustain direction
					velocities->get(e)[0] = prevVelocity[0];
					direction = actors->get(e).direction;
				}
			}
		}

		if (!ud && prevVelocity[1]) {
			float rmndr = positions->get(e)[1] - floorf(positions->get(e)[1]);
			if (rmndr) {
				rmndr = floorf(rmndr + prevVelocity[1] * actors->get(e).speed * delta);
				if (rmndr) {
					positions->get(e)[1] = rmndr < 0 ? floorf(positions->get(e)[1]) : ceilf(positions->get(e)[1]);
				}
				else {
					velocities->get(e)[1] = prevVelocity[1];
					direction = actors->get(e).direction;
				}
			}
		}

		Tileset& tileset = scene->getTileEntityMap().tilesets[sprites->get(e).tileset];

		actors->get(e).moving = direction;
		if (direction) {
			actors->get(e).direction = static_cast<Actor::Direction>(direction);
		}
	}
	
	for (auto e : *velocities) {
		positions->get(e) += velocities->get(e) * actors->get(e).speed * delta;
	}
}