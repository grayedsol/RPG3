/**
 * @file SceneManager.cpp
 * @author Grayedsol (grayedsol@gmail.com)
 * @copyright Copyright (c) 2024
 */
#include "SceneManager.hpp"
#include "GRY_Log.hpp"
#include <assert.h>

SceneManager::~SceneManager() {
	closeAllScenes();
	delete loadingScene;
	delete transition;
}

void SceneManager::process()
{
    if (allScenes.empty()) { GRY_Log("No scenes in the scene manager."); return; }

	allScenes.back()->process();

	processSwitchingScene();
}

void SceneManager::stackScene(Scene *scene) {
	if (!allScenes.empty()) { allScenes.back()->deactivate(); }
	scene->loadAll();
	scene->init();
	scene->activate();
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
	assert(transition == nullptr);
	transition = trns;
	assert(loadingScene == nullptr);
	loadingScene = scene;

	/* Remove player control from current scene */
	allScenes.back()->deactivate();
}

void SceneManager::popScene() {
	if (!allScenes.empty()) {
		delete allScenes.back();
		allScenes.pop_back();
		allScenes.back()->activate();
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
			allScenes.back()->activate();
			delete transition;
			transition = nullptr;
		}
	}
}
