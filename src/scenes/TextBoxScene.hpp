/**
 * @file TextBoxScene.hpp
 * @author Grayedsol (grayedsol@gmail.com)
 * @brief @copybrief TextBoxScene
 * @copyright Copyright (c) 2025
 */
#pragma once
#include "Scene.hpp"
#include "GRY_Texture.hpp"
#include "../textbox/Fontset.hpp"

class GRY_PixelGame;

/**
 * @brief Displays an interactive text box at the bottom of the screen.
 * 
 * Controls:
 * GAME_A: Proceed
 */
class TextBoxScene : public Scene {
private:
	/**
	 * @brief Texture of the message box.
	 * 
	 */
	GRY_Texture boxTexture;

	/**
	 * @brief Texture of the message font.
	 * 
	 */
	Fontset font;

	/**
	 * @copybrief Scene::setControls
	 *
	 */
	void setControls() final;
public:
	/**
	 * @brief Constructor.
	 *
	 * @param game Associated game class.
	 * @param scenePath File path to the scene data.
	 */
	TextBoxScene(GRY_PixelGame *pGame, const char *scenePath);

	/**
	 * @brief Initializes the scene.
	 *
	 */
	void init() final {};

	/**
	 * @copydoc Scene::process
	 */
	void process() final;

	/**
	 * @copydoc Scene::load
	 */
	bool load() final;
};