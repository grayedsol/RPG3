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
		/* Skip actors that don't exist */
		if (!(actors.flags[a] & ActorFlag::ACTOR_EXISTS)) { continue; }

		uint8_t aliveAndReady = ActorFlag::ACTOR_ALIVE | ActorFlag::ACTOR_IDLE;
		/* Update the timers of actors that are alive and not idle */
		if ((actors.flags[a] & aliveAndReady) == ActorFlag::ACTOR_ALIVE) {
			actors.timers[a] -= delta;
			actors.timers[a] = std::max(actors.timers[a], 0.0);
			/* Execute the current action if the timer reached 0 */
			if (!actors.timers[a]) {
				actors.executingActions[a] = actors.actionLists[a].back();
				actors.actionLists[a].pop_back();
				/* Set the actor as idle if there are no more actions in the list */
				actors.flags[a] |= ActorFlag::ACTOR_IDLE * actors.actionLists[a].empty();
			}
		}
	}
}
