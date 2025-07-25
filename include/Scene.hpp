/**
 * @file Scene.hpp
 * @author Grayedsol (grayedsol@gmail.com)
 * @brief @copybrief Scene
 * @copyright Copyright (c) 2024
 */
#pragma once
#include "CommandMap.hpp"

class GRY_Game;

/**
 * @brief Abstract game scene.
 * 
 * @details
 * Examples: Battle, Overworld, Menu.
 * 
 * As an abstract class, it allows for customizing the scene's controls,
 * as well as allowing a SceneManager to control loading and transitioning
 * between scenes.
 * 
 */
class Scene {
protected:
	/**
	 * @brief Associated game class.
	 * 
	 */
	GRY_Game* game;

	/**
	 * @brief Game controls for the scene.
	 * 
	 */
	CommandMap controls;

	/**
	 * @brief File path to scene data.
	 * 
	 */
	const char* scenePath;

	/**
	 * @brief Maps game commands to virtual buttons, and vice versa. Should be called in `init`.
	 * 
	 * @details
	 * The implementation of this function should essentially be
	 * a list of controls mapped using `controls.mapCmd()`.
	 * 
	 * Example:
	 * @code{.cpp}
	 * void ExampleScene::setControls() {
	 * 	controls.mapCmd(GCmd::GameQuit, VirtualButton::GAME_A);
	 * 	controls.mapCmd(GCmd::GameMenu, VirtualButton::GAME_B);
	 * }
	 * @endcode
	 */
	virtual void setControls() = 0;
public:
	/**
	 * @brief Constructor.
	 * 
	 * @param game Associated game class.
	 * @param scenePath File path to scene data.
	 */
	Scene(GRY_Game* game, const char* scenePath);

	/**
	 * @brief Virtual destructor.
	 * 
	 */
	virtual ~Scene();
	
	Scene(const Scene&) = delete;
	Scene& operator=(const Scene&) = delete;

	/**
	 * @brief Get the game instance.
	 * 
	 * @return Pointer to game instance.
	 */
	GRY_Game* getGame() const { return game; }

	/**
	 * @brief Get the file path of the scene data.
	 * 
	 * @return C-String of the scene path. 
	 */
	const char* getScenePath() const { return scenePath; }

	/**
	 * @brief Initialize the scene to a usable state, but without activating player controls.
	 * 
	 * @details
	 * `setControls` should always be called in this function.
	 */
	virtual void init() = 0;

	/**
	 * @brief Update scene.
	 * 
	 */
	virtual void process() = 0;

	/**
	 * @brief Load a resource in the scene.
	 * 
	 * @details
	 * If the scene is already loaded, this function should
	 * return true without doing anything else.
	 * 
	 * @returns `true` if all resources are loaded.
	 * @returns `false` otherwise.
	 */
	virtual bool load() = 0;

	/**
	 * @brief Load all resources in the scene immediately.
	 * 
	 */
	void loadAll() { while (!load()) {} }

	/**
	 * @brief Activate player control in the scene.
	 * 
	 */
	void activateControlScheme();

	void activateControlScheme(CommandMap controlSet);

    /**
     * @brief Remove player control in the scene.
     * 
     */
	void deactivateControlScheme();

	/**
     * @brief Get the latest active GCmd input.
     * 
     * @details
     * Returns GCmd::GCMD_NONE if there are no active inputs.
     * 
     * @return A GCmd input.
	 */
	GCmd readInput();

	/**
     * @brief Get the latest active GCmd input from the current frame only.
     * 
     * @details
     * Returns GCmd::GCMD_NONE if there are no active inputs from the current frame.
     * 
     * @return A GCmd input.
	 */
	GCmd readSingleInput() const;

	/**
	 * @brief Determine if `cmd` is active by checking its associated VirtualButton.
	 * 
	 * @param cmd GCmd to check.
	 * @return `true` if `cmd` is active.
	 * @return `false` otherwise.
	 */
	const bool isPressing(GCmd cmd) const;
};
