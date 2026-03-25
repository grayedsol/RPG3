#include "BattleScene.hpp"

void Battle::BattleScene::setControls() {
	controls.mapCmd(GCmd::BattleUp, VirtualButton::GAME_UP);
	controls.mapCmd(GCmd::BattleDown, VirtualButton::GAME_UP);
	controls.mapCmd(GCmd::BattleLeft, VirtualButton::GAME_UP);
	controls.mapCmd(GCmd::BattleRight, VirtualButton::GAME_UP);
	controls.mapCmd(GCmd::BattleSwitchFighter, VirtualButton::GAME_Y);
}

Battle::BattleScene::BattleScene(GRY_PixelGame *pGame, const char *scenePath, BattleSceneInfo sceneInfo)
	: Scene((GRY_Game *)pGame, scenePath)
	, sceneInfo(sceneInfo)
	, palette(this) {
}

void Battle::BattleScene::init() {}

void Battle::BattleScene::process() {
	palette.process();
}

bool Battle::BattleScene::load() {
	return false;
}