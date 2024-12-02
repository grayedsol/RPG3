#include "TileMapMovement.hpp"
#include "../scenes/TileMapScene.hpp"

static const char* const dirStrings[] = { "", "Down\n", "Up\n", "Left\n", "LeftDown\n", "LeftUp\n", "Right\n", "RightDown\n", "RightUp\n" };

TileMapMovement::TileMapMovement(TileMapScene* scene) :
	scene(scene),
	positions(scene->getECS().getComponent<Position2>()),
	velocities(scene->getECS().getComponent<Velocity2>()),
	actors(scene->getECS().getComponent<Actor>()),
	sprites(scene->getECS().getComponent<ActorSprite>()),
	players(scene->getECSReadOnly().getComponentReadOnly<Player>()) {
}

void TileMapMovement::process(double delta) {
	for (auto e : players) {
		bool r = scene->isPressing(GCmd::MapRight);
		bool lr = scene->isPressing(GCmd::MapLeft) != r;
		bool u = scene->isPressing(GCmd::MapUp);
		bool ud = scene->isPressing(GCmd::MapDown) != u;
		uint8_t direction = lr;
		direction <<= 1;
		direction += lr;
		direction <<= r;
		direction += (ud << u);

		Tileset& tileset = scene->getTileEntityMapNonConst().tilesets[sprites.get(e).tileset];
		positions.get(e)[0] += (scene->isPressing(GCmd::MapRight) - scene->isPressing(GCmd::MapLeft)) * actors.get(e).speed * delta;
		positions.get(e)[1] += (scene->isPressing(GCmd::MapDown) - scene->isPressing(GCmd::MapUp)) * actors.get(e).speed * delta;

		if (direction) {
			sprites.get(e).index = direction;
			tileset.processAnimations(delta);
			actors.get(e).direction = static_cast<Actor::Direction>(direction);
		}
		else {
			tileset.textureIdx[actors.get(e).direction] = actors.get(e).direction;
			// TileAnimation& anim = tileset.tileAnimations[actors.get(e).direction];
			// anim.currentFrame = 0;
			// const TileAnimation::Frame& newFrame = anim.frames[0];
			// tileset.textureIdx[anim.tile] = actors.get(e).direction;
			// anim.timer = newFrame.duration;
		}
		
		// if (scene->isPressing(GCmd::MapDown)) {
		// 	direction += static_cast<uint8_t>(Actor::Direction::Down);
		// }
		// else if (scene->isPressing(GCmd::MapUp)) {
		// 	direction += static_cast<uint8_t>(Actor::Direction::Up);
		// }

		// if (scene->isPressing(GCmd::MapLeft)) {
		// 	direction += static_cast<uint8_t>(Actor::Direction::Left);
		// }
		// else if (scene->isPressing(GCmd::MapRight)) {
		// 	direction += static_cast<uint8_t>(Actor::Direction::Right);
		// }

		// switch (static_cast<Actor::Direction>(direction)) {
		// case Actor::Direction::Down:
		// 	GRY_Log("Down\n"); break;
		// case Actor::Direction::Up:
		// 	GRY_Log("Up\n"); break;
		// case Actor::Direction::Left:
		// 	GRY_Log("Left\n"); break;
		// case Actor::Direction::LeftDown:
		// 	GRY_Log("LeftDown\n"); break;
		// case Actor::Direction::LeftUp:
		// 	GRY_Log("LeftUp\n"); break;
		// case Actor::Direction::Right:
		// 	GRY_Log("Right\n"); break;
		// case Actor::Direction::RightDown:
		// 	GRY_Log("RightDown\n"); break;
		// case Actor::Direction::RightUp:
		// 	GRY_Log("RightUp\n"); break;
		// default:
		// 	break;
		// }
	}
}