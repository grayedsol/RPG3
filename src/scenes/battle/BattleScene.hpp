/**
 * @file BattleScene.hpp
 * @author Grayedsol (grayedsol@gmail.com)
 * @brief @copybrief BattleScene
 * @copyright Copyright (c) 2025
 */
#pragma once
#include "Scene.hpp"
#include "SoundResource.hpp"
#include <stdint.h>

class GRY_PixelGame;

class BattleScene : public Scene {
private:
	uint8_t currentCharacter = 0;
	uint8_t currentPage = 0;

	void setControls() final;
public:
	BattleScene(GRY_PixelGame* pGame, const char* scenePath);

	/**
	 * @copydoc Scene::init
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

	uint8_t getCurrentCharacter() { return currentCharacter; }

	uint8_t getCurrentPage() { return currentPage; }
};