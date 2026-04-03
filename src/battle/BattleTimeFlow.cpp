#include "BattleTimeFlow.hpp"
#include "../scenes/battle/BattleScene.hpp"

Battle::TimeFlow::TimeFlow(BattleScene* scene) : scene(scene) {
}

void Battle::TimeFlow::process(double delta) {
	Actors& actors = scene->getActors();

	for (ActorId a = 0; a < MAX_ACTORS; a++) {
		actors.executingActions[a] = Action();
	}

	for (ActorId a = 0; a < MAX_ACTORS; a++) {
		/* Skip non-existent and dead actors */
		ActorFlags existsAndAlive = ActorFlag::ACTOR_EXISTS | ActorFlag::ACTOR_ALIVE;
		if ((actors.flags[a] & existsAndAlive) != existsAndAlive) { continue; }

		/* Update the action timers of actors that are not idle */
		if (!(actors.flags[a] & ActorFlag::ACTOR_IDLE)) {
			actors.timers[a] -= delta;
			actors.timers[a] = std::max(actors.timers[a], 0.0);
			/* Execute the current action if the timer reached 0 */
			if (!actors.timers[a]) {
				actors.executingActions[a] = actors.actionLists[a].back();
				actors.actionLists[a].pop_back();
				/* Set the actor as idle if there are no more actions in the list */
				if (actors.actionLists[a].empty()) { scene->setActorFlag(a, ActorFlag::ACTOR_IDLE); }
				else { actors.timers[a] = actors.actionLists[a].back().common.data.time; }
			}
		}

		/* Update the status effect timers of all actors */
		std::vector<StatusEffect>& statusEffects = actors.statusEffects[a];
		for (int i = 0; i < statusEffects.size(); i++) {
			StatusEffect& effect = statusEffects.at(i);
			effect.timer -= delta;
			effect.timer = std::max(effect.timer, 0.0);
			/* Remove the status effect if the timer reached 0 */
			if (!effect.timer) {
				std::swap(statusEffects.at(i), statusEffects.back());
				statusEffects.pop_back();
				i--;
			}
		}
	}
}
