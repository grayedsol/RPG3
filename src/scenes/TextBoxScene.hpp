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
#include "../textbox/TextBoxRenderer.hpp"

class GRY_PixelGame;

/**
 * @brief Displays an interactive text box at the bottom of the screen.
 * 
 * Controls:
 * GAME_A: Proceed
 */
class TextBoxScene : public Scene {
private:
	Scene* parentScene;
	/**
	 * @brief Texture of the message box.
	 * 
	 */
	GRY_Texture boxTexture;

	/**
	 * @brief Destination rectangle for the text box.
	 * 
	 */
	SDL_FRect boxTextureArea;

	/**
	 * @brief Texture of the message font.
	 * 
	 */
	Fontset font;

	/**
	 * @brief Renders the text box.
	 * 
	 */
	TextBoxRenderer textBoxRenderer;

	bool done = true;

	/**
	 * @copybrief Scene::setControls
	 *
	 */
	void setControls() final;
public:
	/**
	 * @brief Constructor.
	 *
	 * @param game Associated game class
	 * @param scenePath File path to the scene data
	 * @param parentScene Scene to activate when done
	 */
	TextBoxScene(GRY_PixelGame *pGame, const char *scenePath, Scene* parentScene) :
		Scene((GRY_Game *)pGame, scenePath),
		parentScene(parentScene),
		textBoxRenderer(this) {
	}

	/**
	 * @brief Initializes the scene.
	 *
	 */
	void init() final;

	/**
	 * @copydoc Scene::process
	 */
	void process() final;

	/**
	 * @copydoc Scene::load
	 */
	bool load() final;

	/**
	 * @brief Get the text box texture.
	 * 
	 * @return The text box texture
	 */
	SDL_Texture* getBoxTexture() const { return boxTexture.texture; }

	/**
	 * @brief Get the text box texture area.
	 * 
	 * @return The text box texture area
	 */
	SDL_FRect getBoxTextureArea() const { return boxTextureArea; }

	/**
	 * @brief Get a pointer to the GRY_PixelGame.
	 *
	 * @return Pointer to the GRY_PixelGame.
	 */
	GRY_PixelGame* getPixelGame() const { return (GRY_PixelGame*)game; }
};