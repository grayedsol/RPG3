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
	if (currentFighter == FighterId::MaxFighters) {
		currentFighter = getNextIdleFighter(currentFighter);
		return;
	}

	GCmd cmd = scene->readSingleInput();
	if (!cmd) { return; }

	InputId input = getInputId(cmd, currentPage);

	switch (input) {
	case InputId::InputPass:
		currentFighter = getNextIdleFighter(currentFighter);
		break;
	case InputId::InputAttack:
		beginMove(currentFighter, FighterMove::FIGHTER_ATTACK);
		break;
	case InputId::InputSkill1:
		beginMove(currentFighter, FighterMove::FIGHTER_SKILL1);
		break;
	case InputId::InputSkill2:
		beginMove(currentFighter, FighterMove::FIGHTER_SKILL2);
		break;
	case InputId::InputSkill3:
		beginMove(currentFighter, FighterMove::FIGHTER_SKILL3);
		break;
	case InputId::InputItem:
		switchPage(PageId::ItemPage);
		break;
	case InputId::InputSkillPage:
		switchPage(PageId::SkillPage);
		break;
	case InputId::InputItemDown:
		itemPageDown();
		break;
	case InputId::InputItemUp:
		itemPageUp();
		break;
	case InputId::InputItemToMainPage:
	case InputId::InputSkillToMainPage:
		switchPage(PageId::MainPage);
		break;
	default:
		break;
	}
}

void Battle::Palette::beginMove(FighterId fighter, FighterMove move) {
	scene->getActors().actionLists[fighter] = scene->getFighters().moves[fighter][move];
	scene->unsetActorFlag(fighter, ActorFlag::ACTOR_IDLE);
	currentFighter = getNextIdleFighter(currentFighter);
}

void Battle::Palette::itemPageUp() {}

void Battle::Palette::itemPageDown() {}

Battle::FighterId Battle::Palette::getNextIdleFighter(FighterId afterFighter) {
	ActorFlags* flags = scene->getActors().flags;
	FighterId nextFighter = static_cast<FighterId>(afterFighter + 1);
	for (uint8_t i = 0; i < FighterId::MaxFighters; i++) {
		if (nextFighter >= FighterId::MaxFighters) { nextFighter = FighterId::Fighter0; }
		if (flags[nextFighter] & ActorFlag::ACTOR_IDLE) { return nextFighter; }
		nextFighter = static_cast<FighterId>(nextFighter + 1);
	}
	return FighterId::MaxFighters;
}
