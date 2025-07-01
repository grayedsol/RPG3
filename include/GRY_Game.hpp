/**
 * @file GRY_Game.hpp
 * @author Grayedsol (grayedsol@gmail.com)
 * @brief @copybrief GRY_Game
 * @copyright Copyright (c) 2024
 */
#pragma once
#include "GRY_Video.hpp"
#include "SceneManager.hpp"
#include "FPSHandler.hpp"
#include "InputHandler.hpp"
#include "GRY_Audio.hpp"
#include "imguiDebugger.hpp"

/**
 * @brief Runs the game loop and provides access to essential game functions.
 * 
 * @details
 * Useful as a dependency injection, such as in the Scene class.
 */
class GRY_Game {
protected:
	/**
	 * @copybrief GRY_Video
	 * 
	 */
	GRY_Video video;

	/**
	 * @copybrief GRY_Audio
	 * 
	 */
	GRY_Audio audio;

	/**
	 * @copybrief FPSHandler
	 * 
	 */
	SceneManager scenes;

	/**
	 * @copybrief SceneManager
	 * 
	 */
	FPSHandler fps;

	/**
	 * @copybrief InputHandler
	 * 
	 */
	InputHandler input;

	/**
	 * @copybrief imguiDebugger
	 * 
	 */
	imguiDebugger imguiDebug;

	/**
	 * @brief Game running status. When false, the game will exit at the end of the current frame.
	 * 
	 */
	bool gameRunning = true;
public:
	/**
	 * @brief Constructor.
	 * 
	 * @param WINDOW_WIDTH Screen width in pixels.
	 * @param WINDOW_HEIGHT Screen height in pixels.
	 * @param MAX_FPS Maximum frames per second the game should run at.
	 * @param USE_VSYNC Whether the game will use VSync, `true` by default.
	 */
	GRY_Game(int WINDOW_WIDTH, int WINDOW_HEIGHT, int MAX_FPS, bool USE_VSYNC = true);

	GRY_Game(const GRY_Game&) = delete;
	GRY_Game& operator=(const GRY_Game&) = delete;

	/**
	 * @brief Start the game.
	 * 
	 */
	void runGame();

	/**
	 * @brief Quit the game.
	 * 
	 * @details
	 * The game loop will stop running at the end of the current frame.
	 * 
	 */
	void quit() { gameRunning = false; }

	/**
	 * @brief Update function called every frame, regardless of the game's state.
	 * 
	 */
	virtual void process();

	/**
	 * @brief Get the internal GRY_Video.
	 * 
	 * @return Reference to the GRY_SDL.
	 */
	GRY_Video& getVideo() { return video; }

	/**
	 * @copydoc SceneManager::stackScene
	*/
	void stackScene(Scene* scene) { scenes.stackScene(scene); }

	/**
	 * @copydoc SceneManager::switchScene
	 */
	void switchScene(Scene* scene, Transition* trns) { scenes.switchScene(scene, trns); }

	/**
	 * @copydoc FPSHandler::getDelta
	 */
	double getDelta() { return fps.getDelta(); }

	/**
	 * @copydoc FPSHandler::getFPS
	 */
	double getFPS() { return fps.getFPS(); }

	/**
	 * @copydoc InputHandler::setControlScheme
	 */
	void setControlScheme(CommandMap map) { input.setControlScheme(map); }

	/**
	 * @brief Reset the control scheme.
	 * 
	 */
	void resetControlScheme() { input.setControlScheme(CommandMap()); }

	/**
	 * @copydoc InputHandler::getInput
	 */
	const GCmd getInput() { return input.getInput(); }

	/**
	 * @copydoc InputHandler::getSingleInput
	 */
	const GCmd getSingleInput() { return input.getSingleInput(); }

	/**
	 * @copydoc InputHandler::isPressing
	 */
	const bool isPressing(GCmd cmd) { return input.isPressing(cmd); }

	/**
	 * @brief Get the internal GRY_Audio.
	 * 
	 * @return Reference to the GRY_Audio
	 */
	GRY_Audio& getAudio() { return audio; }

	/**
	 * @brief Determine whether the debug menu is active or not.
	 * 
	 * @return `true` if the debug menu is on.
	 * @return `false` otherwise.
	 */
	const bool debugMenuIsOn() { return imguiDebug.active; }
};
