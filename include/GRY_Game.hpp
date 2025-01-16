/**
 * @file GRY_Game.hpp
 * @author Grayedsol (grayedsol@gmail.com)
 * @brief @copybrief GRY_Game
 * @copyright Copyright (c) 2024
 */
#pragma once
#include "GRY_SDL.hpp"
#include "SceneManager.hpp"
#include "FPSHandler.hpp"
#include "InputHandler.hpp"
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
	 * @copybrief GRY_SDL
	 * 
	 */
	GRY_SDL gsdl;

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
	 * @param TARGET_FPS Desired frames per second the game should run at.
	 * @param USE_VSYNC Whether the game will use VSync, `true` by default.
	 */
	GRY_Game(int WINDOW_WIDTH, int WINDOW_HEIGHT, int TARGET_FPS, bool USE_VSYNC = true);

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
	 * @brief Get the internal GRY_SDL.
	 * 
	 * @return Reference to the GRY_SDL.
	 */
	GRY_SDL& getSDL() { return gsdl; }

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
	 * @copydoc InputHandler::getInput
	 */
	const VirtualButton getInput() { return input.getInput(); }

   	/**
	 * @copydoc InputHandler::getSingleInput
	 */
	const VirtualButton getSingleInput() { return input.getSingleInput(); }

	/**
	 * @copydoc InputHandler::isPressing
	 */
	const bool isPressing(VirtualButton b) { return input.isPressing(b); }

	const bool debugModeOn() { return imguiDebug.active; }
};
