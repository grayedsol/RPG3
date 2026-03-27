#include "BattleScene.hpp"
#include "GRY_PixelGame.hpp"

void Battle::BattleScene::setControls() {
	controls.mapCmd(GCmd::BattleUp, VirtualButton::GAME_UP);
	controls.mapCmd(GCmd::BattleDown, VirtualButton::GAME_UP);
	controls.mapCmd(GCmd::BattleLeft, VirtualButton::GAME_UP);
	controls.mapCmd(GCmd::BattleRight, VirtualButton::GAME_UP);
}

Battle::BattleScene::BattleScene(GRY_PixelGame *pGame, const char *scenePath, BattleSceneInfo sceneInfo)
	: Scene((GRY_Game *)pGame, scenePath)
	, sceneInfo(sceneInfo)
	, palette(this)
	, paletteRenderer(this)
	, timeFlow(this) {
}

void Battle::BattleScene::init() {}

void Battle::BattleScene::process() {
	double delta = game->getDelta();

	palette.process();
	timeFlow.process(delta);

	for (ActorId a = 0; a < MAX_ACTORS; a++) { if (actors.healthPoints[a] = 0) { killActor(a); } }

	paletteRenderer.process();
}

bool Battle::BattleScene::load() {
	return true;
}

void Battle::BattleScene::setActorFlag(ActorId actor, ActorFlag flag) {
	actors.flags[actor] |= flag;
}

void Battle::BattleScene::unsetActorFlag(ActorId actor, ActorFlag flag) {
	actors.flags[actor] &= ~flag;
}

void Battle::BattleScene::killActor(ActorId actor) {
	actors.flags[actor] &= ~(ActorFlag::ACTOR_ALIVE | ActorFlag::ACTOR_IDLE);
	actors.healthPoints[actor] = 0; /* Redundant unless this is called before checking if health is 0 */
	actors.timers[actor] = 0.0;
	actors.statusEffects[actor].clear();
	actors.actionLists[actor].clear();
}
