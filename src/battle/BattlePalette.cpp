#include "BattlePalette.hpp"
#include "../scenes/battle/BattleScene.hpp"

Battle::BattlePalette::BattlePalette(BattleScene *scene, PageId &currentPage)
	: scene(scene)
	, currentPage(currentPage) {
}

Battle::InputId getInputId(GCmd cmd, Battle::PageId currentPage) {
	using namespace Battle;
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

	if (direction == DirectionId::DirectionIdSize) { return InputId::InputIdSize; }

	InputId input = static_cast<InputId>((currentPage << 2) + direction);
	GRY_Assert(input < InputIdSize, "Invalid battle input state.");

	return input;
}

void Battle::BattlePalette::process() {
	GCmd cmd = scene->readSingleInput();

	InputId input = getInputId(cmd, currentPage);

	switch (input) {
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
	default:
		break;
	}
}

void Battle::BattlePalette::goToMainPage() { currentPage = PageId::MainPage; }

void Battle::BattlePalette::goToItemPage() { currentPage = PageId::ItemPage; }

void Battle::BattlePalette::goToSkillPage() { currentPage = PageId::SkillPage; }

void Battle::BattlePalette::itemPageUp() {}

void Battle::BattlePalette::itemPageDown() {}
