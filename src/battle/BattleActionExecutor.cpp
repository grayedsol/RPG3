#include "BattleActionExecutor.hpp"
#include "../scenes/battle/BattleScene.hpp"

Battle::ActionExecutor::ActionExecutor(BattleScene* scene) : scene(scene) {
}

void Battle::ActionExecutor::process() {
	Action* actions = scene->getActors().executingActions;

	for (ActorId a = 0; a < MAX_ACTORS; a++) {
		Action& action = actions[a];
		switch (action.data.type) {
			case ActionType::NO_ACTION:
				break;
			case ActionType::ATTACK_ACTION:
				break;
			case ActionType::HEAL_ACTION:
				break;
			case ActionType::BUFF_DEBUFF_ACTION:
				break;
			case ActionType::MULTI_ATTACK_ACTION:
				break;
			case ActionType::MULTI_HEAL_ACTION:
				break;
			case ActionType::MULTI_BUFF_DEBUFF_ACTION:
				break;
			case ActionType::ANIMATION_ACTION:
				break;
		}
	}
}