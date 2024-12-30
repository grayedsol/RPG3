/**
 * @file TileMapMovement.cpp
 * @author Grayedsol (grayedsol@gmail.com)
 * @copyright Copyright (c) 2024
 */
#include "TileMapMovement.hpp"
#include "../scenes/TileMapScene.hpp"

static Velocity2 dirVecs[Actor::Direction::SIZE] = {
	Velocity2{0,0},
	Velocity2{0,1},
	Velocity2{0,-1},
	Velocity2{-1,0},
	Velocity2{-1,1},
	Velocity2{-1,-1},
	Velocity2{1,0},
	Velocity2{1,1},
	Velocity2{1,-1}
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

		/* If the actor was moving horizontally last frame but not this one */
		if (prevVelocity[0] && !velocities->get(e)[0]) {
			/* Get the decimal part of the x coordinate */
			float rmndr = fabsf(positions->get(e)[0] - floorf(positions->get(e)[0]));
			if (rmndr) { /* If it's non-zero, it's mid-pixel and we want to glide */
				/* Try an incremental move, and floor it */
				rmndr = floorf(rmndr + prevVelocity[0] * (actors->get(e).speed * delta));
				if (rmndr) { /* If it did not floor to 0, it escaped the range 0-1, so it crossed over a pixel */
					/* Snap to the pixel it crossed, using either floor or ceil */
					positions->get(e)[0] = prevVelocity[0] < 0 ? floorf(positions->get(e)[0]) : ceilf(positions->get(e)[0]);
				}
				else { /* If it did floor to 0, it's still mid-pixel, so glide it */
					velocities->get(e)[0] = prevVelocity[0];
				}
			}
		}

		/* Repeat for y coordinate */
		if (prevVelocity[1] && !velocities->get(e)[1]) {
			float rmndr = fabsf(positions->get(e)[1] - floorf(positions->get(e)[1]));
			if (rmndr) {
				rmndr = floorf(rmndr + prevVelocity[1] * actors->get(e).speed * delta);
				if (rmndr) {
					positions->get(e)[1] = prevVelocity[1] < 0 ? floorf(positions->get(e)[1]) : ceilf(positions->get(e)[1]);
				}
				else {
					velocities->get(e)[1] = prevVelocity[1];
				}
			}
		}
	}
	
	for (auto e : *velocities) {
		positions->get(e) += velocities->get(e) * actors->get(e).speed * (1 + actors->get(e).sprinting) * delta;
	}
}