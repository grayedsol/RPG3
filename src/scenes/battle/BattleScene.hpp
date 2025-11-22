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

struct BattleSceneInfo {

};

class BattleScene : public Scene {
private:
	BattleSceneInfo sceneInfo;

	enum InputId : uint8_t {
		Flee = 0,
		Attack = 1,
		Item = 2,
		Skill = 3,
		ItemDown = 4,
		ItemUp = 5,
		ItemSelect = 6,
		ItemToMain = 7,
		Skill3 = 8,
		Skill1 = 9,
		SkillToMain = 10,
		Skill2 = 11,
		InputIdSize = 12
	};

	enum PageId : uint8_t {
		MainPage = 0,
		ItemPage = 1,
		SkillPage = 2
	};

	enum DirectionId : uint8_t {
		Down = 0,
		Up = 1,
		Left = 2,
		Right = 3,
		DirectionIdSize = 4
	};

	uint8_t currentCharacter = 0;
	uint8_t currentPage = 0;

	void setControls() final;

	void processInput();

	void flee();

	void attack();
	
	void goToItemPage();

	void goToSkillPage();

	void goToMainPage();

	void itemPageUp();
	void itemPageDown();
	void itemPageSelect();
	
	void useSkill1();
	void useSkill2();
	void useSkill3();
public:
	BattleScene(GRY_PixelGame* pGame, const char* scenePath, BattleSceneInfo sceneInfo);

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