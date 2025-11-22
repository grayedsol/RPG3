#include "BattleScene.hpp"

void BattleScene::setControls() {
	controls.mapCmd(GCmd::BattleUp, VirtualButton::GAME_UP);
	controls.mapCmd(GCmd::BattleDown, VirtualButton::GAME_UP);
	controls.mapCmd(GCmd::BattleLeft, VirtualButton::GAME_UP);
	controls.mapCmd(GCmd::BattleRight, VirtualButton::GAME_UP);
}

void BattleScene::processInput() {
	GCmd cmd = readSingleInput();

	DirectionId direction = DirectionId::DirectionIdSize;
	switch (cmd) {
	case GCmd::BattleDown:
		direction = DirectionId::Down;
		break;
	case GCmd::BattleUp:
		direction = DirectionId::Up;
		break;
	case GCmd::BattleLeft:
		direction = DirectionId::Left;
		break;
	case GCmd::BattleRight:
		direction = DirectionId::Right;
		break;
	default:
		break;
	}

	if (direction == DirectionId::DirectionIdSize) { return; }

	InputId input = static_cast<InputId>((currentPage << 2) + direction);
	GRY_Assert(input < InputIdSize, "Invalid battle input state.");

	switch (input) {
	case InputId::Flee:
		flee();
		break;
	case InputId::Attack:
		attack();
		break;
	case InputId::Item:
		goToItemPage();
		break;
	case InputId::Skill:
		goToSkillPage();
		break;
	case InputId::ItemDown:
		itemPageDown();
		break;
	case InputId::ItemUp:
		itemPageUp();
		break;
	case InputId::ItemToMain:
	case InputId::SkillToMain:
		goToMainPage();
		break;
	case InputId::Skill3:
		useSkill3();
		break;
	case InputId::Skill1:
		useSkill1();
		break;
	case InputId::Skill2:
		useSkill2();
		break;
	default:
		break;
	}
}

void BattleScene::flee() {
}

void BattleScene::attack() {
}

void BattleScene::goToItemPage() {
}

void BattleScene::goToSkillPage() {
}

void BattleScene::goToMainPage() {
}

void BattleScene::itemPageUp() {
}

void BattleScene::itemPageDown() {
}

void BattleScene::itemPageSelect() {
}

void BattleScene::useSkill1() {
}

void BattleScene::useSkill2() {
}

void BattleScene::useSkill3() {
}

BattleScene::BattleScene(GRY_PixelGame *pGame, const char *scenePath, BattleSceneInfo sceneInfo) : Scene((GRY_Game *)pGame, scenePath),
																								   sceneInfo(sceneInfo) {
}

void BattleScene::init() {}

void BattleScene::process() {}

bool BattleScene::load() {
	return false;
}