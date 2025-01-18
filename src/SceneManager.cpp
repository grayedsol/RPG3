/**
 * @file SceneManager.cpp
 * @author Grayedsol (grayedsol@gmail.com)
 * @copyright Copyright (c) 2024
 */
#include "SceneManager.hpp"
#include "GRY_Log.hpp"

SceneManager::~SceneManager() {
	closeAllScenes();
	delete loadingScene;
	delete transition;
}

void SceneManager::process() {
    if (allScenes.empty()) { GRY_Log("No scenes in the scene manager."); return; }

	allScenes.back()->process();

	processSwitchingScene();
}

void SceneManager::stackScene(Scene *scene) {
	scene->loadAll();
	scene->init();
	scene->activateControlScheme();
	allScenes.push_back(scene);
}

void SceneManager::switchScene(Scene *scene, Transition *trns) {
	if (allScenes.empty()) {
		GRY_Log("Tried to switch out to a new scene, but there were no scenes to begin with.");
		GRY_Log("Stacking scene instead, and deleting transition.");
		stackScene(scene);
		delete transition;
		return;
	}

	/* Add transition, it will start processing this frame */
	GRY_Assert(transition == nullptr, "[SceneManager] Transition was nullptr.");
	transition = trns;
	GRY_Assert(loadingScene == nullptr, "[SceneManager] Loading scene was nullptr.");
	loadingScene = scene;

	/* Remove player control from current scene */
	allScenes.back()->deactivateControlScheme();
}

void SceneManager::popScene() {
	if (!allScenes.empty()) {
		delete allScenes.back();
		allScenes.pop_back();
		allScenes.back()->activateControlScheme();
	}
	else { GRY_Log("Tried to pop scene, but there were none."); }
}

void SceneManager::closeAllScenes() {
	for (auto& scene : allScenes) {
		delete scene;
		scene = nullptr;
	}
	allScenes.clear();
}

void SceneManager::processSwitchingScene() {
	/* If there is no transition, there is no loading scene either, so just return */
	if (!transition) { return; }

	if (loadingScene) {
		transition->process();
		/* Wait for transition to finish init phase */
		if (transition->isReadyToRelease()) {
			/* Wait for scene to load */
			if (loadingScene->load()) {
				/* Delete and replace active scene */
				delete allScenes.back();
				allScenes.back() = loadingScene;
				loadingScene = nullptr;
				allScenes.back()->init();
				transition->release();
			}
		}
	}
	/* When we get here, it means the transition is in its release phase */
	else {
		/* Wait for it to finish completely */
		if (transition->process()) {
			/* Give player control to the newly active scene and clean up transition */
			allScenes.back()->activateControlScheme();
			delete transition;
			transition = nullptr;
		}
	}
}
