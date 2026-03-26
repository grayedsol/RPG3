#include "BattlePalette.hpp"
#include "../scenes/battle/BattleScene.hpp"

Battle::Palette::Palette(BattleScene* scene)
	: scene(scene) {
}

static Battle::InputId getInputId(GCmd cmd, Battle::PageId currentPage) {
	Battle::DirectionId direction = Battle::DirectionId::DirectionIdSize;
	switch (cmd) {
	case GCmd::BattleDown:
		direction = Battle::DirectionId::Down;
		break;
	case GCmd::BattleUp:
		direction = Battle::DirectionId::Up;
		break;
	case GCmd::BattleLeft:
		direction = Battle::DirectionId::Left;
		break;
	case GCmd::BattleRight:
		direction = Battle::DirectionId::Right;
		break;
	default:
		break;
	}

	if (direction == Battle::DirectionId::DirectionIdSize) { return Battle::InputId::InputIdSize; }

	Battle::InputId input = static_cast<Battle::InputId>((currentPage << 2) + direction);
	GRY_Assert(input < Battle::InputId::InputIdSize, "Invalid battle input state.");

	return input;
}

void Battle::Palette::process() {
	if (currentFighter == FighterId::MaxFighters) { return; }

	GCmd cmd = scene->readSingleInput();

	if (cmd == GCmd::BattleSwitchFighter) {
		switchFighters();
		return;
	}

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

void Battle::Palette::goToMainPage() { currentPage = PageId::MainPage; }

void Battle::Palette::goToItemPage() { currentPage = PageId::ItemPage; }

void Battle::Palette::goToSkillPage() { currentPage = PageId::SkillPage; }

void Battle::Palette::itemPageUp() {}

void Battle::Palette::itemPageDown() {}

void Battle::Palette::switchFighters() {}
