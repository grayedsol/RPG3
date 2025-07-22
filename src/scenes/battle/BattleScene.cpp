#include "BattleScene.hpp"

void BattleScene::setControls() {
	controls.mapCmd(GCmd::BattleUp, VirtualButton::GAME_UP);
	controls.mapCmd(GCmd::BattleDown, VirtualButton::GAME_UP);
	controls.mapCmd(GCmd::BattleLeft, VirtualButton::GAME_UP);
	controls.mapCmd(GCmd::BattleRight, VirtualButton::GAME_UP);
}

BattleScene::BattleScene(GRY_PixelGame* pGame, const char* scenePath) : Scene((GRY_Game*)pGame, scenePath) {
}

void BattleScene::init() {}

void BattleScene::process() {}

bool BattleScene::load() {
	return false;
}