#include "BattleActionExecutor.hpp"
#include "../scenes/battle/BattleScene.hpp"

Battle::ActionExecutor::ActionExecutor(BattleScene* scene) : scene(scene) {
}

void Battle::ActionExecutor::process() {
	Action* actions = scene->getActors().executingActions;

	for (ActorId a = 0; a < MAX_ACTORS; a++) {
		Action& action = actions[a];
		switch (action.common.data.type) {
			case ActionType::NO_ACTION:
				break;
			case ActionType::WAIT_ACTION:
				break;
			case ActionType::ATTACK_ACTION:
				break;
			case ActionType::HEAL_ACTION:
				break;
			case ActionType::STATUS_EFFECT_ACTION:
				break;
			case ActionType::MULTI_ATTACK_ACTION:
				break;
			case ActionType::MULTI_HEAL_ACTION:
				break;
			case ActionType::MULTI_STATUS_EFFECT_ACTION:
				break;
			case ActionType::ANIMATION_ACTION:
				ActionAnimationId id = action.animationAction.animationId;
				scene->getActors().fx[action.animationAction.data.source] = FX{0, id, 0.125, 0.125};
				break;
		}
	}
}
