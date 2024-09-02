/**
 * @file SceneManager.hpp
 * @author Grayedsol (grayedsol@gmail.com)
 * @brief @copybrief SceneManager
 * @copyright Copyright (c) 2024
 */
#pragma once
#include "Scene.hpp"
#include "Transition.hpp"
#include <vector>

/**
 * @brief Stores, loads, deletes, and switches between scenes.
 * 
 * @details
 * Only one scene is active at a time, that being the last scene to be added.
 */
class SceneManager {
private:
	friend class GRY_Game;

	/**
	 * @brief Stack-like containing all the game's scenes.
	 * 
	 */
	std::vector<Scene*> allScenes;

	/**
	 * @brief Scene that is loading but not yet active.
	 * 
	 */
	Scene* loadingScene = nullptr;

	/**
	 * @brief Current transition between two scenes.
	 * 
	 */
	Transition* transition = nullptr;
public:
	/**
	 * @brief Constructor.
	 * 
	 */
	SceneManager() = default;

	/**
	 * @brief Destructor.
	 * 
	 */
	~SceneManager();
	
	SceneManager(const SceneManager&) = delete;
	SceneManager& operator=(const SceneManager&) = delete;

	/**
	 * @brief Update active scene and process any transitions.
	 * 
	 */
	void process();
	
	/**
	 * @brief Add a scene, making it the active scene.
	 * 
	 * @param scene The new scene
	 */
	void stackScene(Scene* scene);

	/**
	 * @brief Switch out the active scene with a new one, using a transtion.
	 * 
	 * @details
	 * Deletes the old scene.
	 * 
	 * @param scene The new scene.
	 * @param trns Transition to use when switching.
	 */
	void switchScene(Scene* scene, Transition* trns);

	/**
	 * @brief Delete the current scene.
	 * 
	 */
	void popScene();
	
	/**
	 * @brief Delete all scenes.
	 * 
	 */
	void closeAllScenes();
private:
	/**
	 * @brief Process any loading scenes and transitions.
	 * 
	 */
	void processSwitchingScene();
};