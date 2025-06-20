/**
 * @file TextDecisionScene.hpp
 * @author Grayedsol (grayedsol@gmail.com)
 * @brief @copybrief TextDecisionScene
 * @copyright Copyright (c) 2025
 */
#pragma once
#include "Scene.hpp"
#include "GRY_Texture.hpp"
#include "../textbox/Fontset.hpp"

class GRY_PixelGame;
class TextBoxScene;
struct SDL_Renderer;

/**
 * @brief Displays a Yes/No choice in a text box for the player to select.
 * 
 * @details
 * Controls:
 * GAME_A: Confirm selection
 * GAME_B: Switch selection to "No"
 * GAME_UP/GAME_DOWN: Switch selection
 */
class TextDecisionScene : public Scene {
private:
	TextBoxScene* scene;

	GRY_Texture boxTexture;

	SDL_FRect boxTextureArea;

	SDL_FRect textArea;

	SDL_Renderer* renderer;

	/**
	 * @brief Scaling factor for the box texture.
	 * 
	 */
	const float* pixelScaling;

	enum { NONE = 0, YES = 1, NO = 2 } selection = NONE;

	char yesNo[3][5] = { ">", " Yes", " No" };

	bool active = false;
	bool decisionMade = false;

	/**
	 * @copybrief Scene::setControls
	 *
	 */
	void setControls() final;
public:
	TextDecisionScene(GRY_PixelGame *pGame, const char *scenePath, TextBoxScene* scene);

	/**
	 * @brief Initializes the scene.
	 *
	 */
	void init() final;

	/**
	 * @copydoc Scene::process
	 * 
	 */
	void process() final;

	/**
	 * @copydoc Scene::load
	 */
	bool load() final;

	/**
	 * @brief Opens and activates the decision box.
	 * 
	 */
	void open();

	/**
	 * @brief Closes the decision box and activates the parent scene.
	 * 
	 */
	void close();

	bool isOpen() { return active; }

	bool decisionIsMade() { return decisionMade; }

	unsigned getDecision() { return selection; }
};