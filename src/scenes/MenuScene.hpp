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
	struct MenuRenderInfo {
		GRY_Texture boxTexture;
		SDL_FRect boxTextureArea;
		SDL_FRect textArea;
		char** selectionStrings;
		int numRows;
		int numCols;
	} renderInfo;

	Scene* scene;

	uint8_t selection;

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

	void renderMenu(const MenuRenderInfo& info);
public:
	MenuScene(GRY_PixelGame* pGame, const char* path, Scene* scene);

	~MenuScene();

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

	void open();

	void close();
};