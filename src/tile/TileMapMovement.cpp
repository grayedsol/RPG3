#include "TileMapMovement.hpp"
#include "../scenes/TileMapScene.hpp"

TileMapMovement::TileMapMovement(TileMapScene* scene) :
	scene(scene),
	positions(scene->getECS().getComponent<Position2>()),
	velocities(scene->getECS().getComponent<Velocity2>()),
	players(scene->getECSReadOnly().getComponentReadOnly<Player>()) {
}

void TileMapMovement::process() {
	for (auto e : players) {
		uint8_t direction = 0;

		if (scene->isPressing(GCmd::MapDown)) {
			direction += static_cast<uint8_t>(Actor::Direction::Down);
		}
		else if (scene->isPressing(GCmd::MapUp)) {
			direction += static_cast<uint8_t>(Actor::Direction::Up);
		}

		if (scene->isPressing(GCmd::MapLeft)) {
			direction += static_cast<uint8_t>(Actor::Direction::Left);
		}
		else if (scene->isPressing(GCmd::MapRight)) {
			direction += static_cast<uint8_t>(Actor::Direction::Right);
		}

		switch (static_cast<Actor::Direction>(direction)) {
		case Actor::Direction::Down:
			GRY_Log("Down\n"); break;
		case Actor::Direction::Up:
			GRY_Log("Up\n"); break;
		case Actor::Direction::Left:
			GRY_Log("Left\n"); break;
		case Actor::Direction::LeftDown:
			GRY_Log("LeftDown\n"); break;
		case Actor::Direction::LeftUp:
			GRY_Log("LeftUp\n"); break;
		case Actor::Direction::Right:
			GRY_Log("Right\n"); break;
		case Actor::Direction::RightDown:
			GRY_Log("RightDown\n"); break;
		case Actor::Direction::RightUp:
			GRY_Log("RightUp\n"); break;
		default:
			break;
		}
	}
}