/**
 * @file BattleScene.hpp
 * @author Grayedsol (grayedsol@gmail.com)
 * @brief @copybrief BattleScene
 * @copyright Copyright (c) 2025
 */
#pragma once
#include "Scene.hpp"
#include "SoundResource.hpp"

class GRY_PixelGame;

class BattleScene : public Scene {
private:
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
};