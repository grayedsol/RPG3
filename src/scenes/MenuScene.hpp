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
private:
	GRY_Texture boxTexture;

	SDL_FRect boxTextureArea;

	SDL_FRect textArea;

	char** selectionStrings;

	uint16_t numRows;

	uint16_t numCols;

	SDL_Renderer* renderer;

	const float* pixelScaling;

	bool active = false;

	Fontset font;

	/**
	 * @copybrief Scene::setControls
	 * 
	 */
	void setControls();

	void setSelection(uint8_t selectionValue);

	void renderMenu();

	virtual void makeSelection();
protected:
	Scene* scene;

	MenuScene* subMenu = nullptr;

	uint8_t selection;
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
	void process() override;

	/**
	 * @copydoc Scene::load
	 */
	bool load() override;

	void open();

	void close();

	bool isOpen() { return active; }
};
