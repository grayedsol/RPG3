#include "BattleScene.hpp"
#include "GRY_PixelGame.hpp"
#include "GRY_JSON.hpp"

void Battle::BattleScene::setControls() {
	controls.mapCmd(GCmd::BattleUp, VirtualButton::GAME_UP);
	controls.mapCmd(GCmd::BattleDown, VirtualButton::GAME_DOWN);
	controls.mapCmd(GCmd::BattleLeft, VirtualButton::GAME_LEFT);
	controls.mapCmd(GCmd::BattleRight, VirtualButton::GAME_RIGHT);
}

Battle::BattleScene::BattleScene(GRY_PixelGame *pGame, const char *scenePath, BattleSceneInfo sceneInfo)
	: Scene((GRY_Game *)pGame, scenePath)
	, sceneInfo(sceneInfo)
	, palette(this)
	, paletteRenderer(this)
	, timeFlow(this)
	, fxAnimator(this, actors.fx, fxResources)
	, actionExecutor(this) {
}

void Battle::BattleScene::init() {
	setControls();

	setActorFlag(Fighter0, ActorFlag::ACTOR_EXISTS);
	setActorFlag(Fighter0, ActorFlag::ACTOR_ALIVE);
	setActorFlag(Fighter0, ActorFlag::ACTOR_IDLE);
	AnimationAction animationAction;
	animationAction.data.source = Fighter0;
	animationAction.data.time = 0;
	animationAction.animationId = 0;
	animationAction.target = 0;
	Action action;
	action.animationAction = animationAction;
	Action wait;
	WaitAction waitAction;
	waitAction.data.time = 1;
	wait.waitAction = waitAction;
	std::vector<Action> attack = { wait, action };
	fighters.moves[Fighter0][FIGHTER_ATTACK] = attack;
	actors.healthPoints[Fighter0] = 10;
}

void Battle::BattleScene::process() {
	double delta = game->getDelta();

	for (ActorId a = 0; a < MAX_ACTORS; a++) { if (actors.healthPoints[a] == 0) { killActor(a); } }

	palette.process();
	actionExecutor.process();
	timeFlow.process(delta);
	fxAnimator.process(delta);

	paletteRenderer.process();
	fxAnimator.render();
}

bool Battle::BattleScene::load() {
	if (!fxResources.empty()) {
		for (auto& fxResource : fxResources) {
			if (!fxResource.load(game)) { return false; }
		}
		return true;
	}

	/* Open scene document */
    GRY_JSON::Document sceneDoc;
    GRY_JSON::loadDoc(sceneDoc, scenePath);

	if (sceneDoc["fxResources"].GetArray().Size() > 0) {
		for (auto& fxResource : sceneDoc["fxResources"].GetArray()) {
			fxResources.push_back(FXResource(fxResource.GetString()));
		}
	}

	return sceneDoc["fxResources"].GetArray().Size() == 0;
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
