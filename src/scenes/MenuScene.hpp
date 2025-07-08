/**
 * @file MenuScene.hpp
 * @author Grayedsol (grayedsol@gmail.com)
 * @brief @copybrief MenuScene
 * @copyright Copyright (c) 2025
 */
#pragma once
#include "Scene.hpp"
#include "GRY_Texture.hpp"
#include "../textbox/Fontset.hpp"

class GRY_PixelGame;
struct SDL_Renderer;

class MenuScene : public Scene {
protected:
	Scene* parentScene;
private:
	char** selectionStrings;

	GRY_Texture boxTexture;

	SDL_FRect boxTextureArea;

	SDL_FRect textArea;

	uint16_t numRows;

	uint16_t numCols;

	uint8_t selection = 0;

	bool active = false;

	/**
	 * @copybrief Scene::setControls
	 * 
	 */
	void setControls() final;

	void setSelection(uint8_t selectionValue);

	virtual void makeSelection(uint8_t selection) = 0;
protected:
	void renderMenu(const Fontset& font);

	/**
	 * @brief Select/confirm a menu option, or close the menu, based on input.
	 * 
	 * @return true if the menu was not closed.
	 * @return false if the menu was closed.
	 */
	bool handleInput();
public:
	MenuScene(GRY_PixelGame* pGame, const char* path, Scene* scene);

	~MenuScene();

	/**
	 * @brief Initializes the scene.
	 *
	 */
	void init() override;

	/**
	 * @copydoc Scene::process
	 * 
	 */
	virtual void process() = 0;

	/**
	 * @copydoc Scene::load
	 */
	bool load() override;

	void open();

	void close();

	bool isOpen() { return active; }
};
