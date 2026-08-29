/**
 * @file main.cpp
 * @author Grayedsol (grayedsol@gmail.com)
 * @copyright Copyright (c) 2024
 */
#include <stdio.h>
#include "GRY_PixelGame.hpp"
#include "scenes/TilesetScene.hpp"
#include "scenes/TileMapScene.hpp"
#include "scenes/battle/BattleScene.hpp"

int WINDOW_WIDTH = 960;
int WINDOW_HEIGHT = 540;
int MAX_FPS = 144;
bool USE_VSYNC = true;

static GRY_PixelGame game(WINDOW_WIDTH, WINDOW_HEIGHT, MAX_FPS, USE_VSYNC);

const char* scenePath = "assets/tilemapscene/map02/scene.json";
const char* battleScenePath = "assets/battlescene/scene.json";

Tile::MapScene* scene = new Tile::MapScene(&game, scenePath);
Battle::BattleScene* battleScene = new Battle::BattleScene(&game, battleScenePath, Battle::BattleSceneInfo());

#ifdef _WIN32
#ifndef NOMINMAX
#define NOMINMAX
#endif
#include <Windows.h>
#include <libloaderapi.h>
_Use_decl_annotations_ int WINAPI WinMain(
	HINSTANCE hInstance,
	HINSTANCE hPrevInstance,
	LPSTR lpCmdLine,
	int nCmdShow) {
	
	/* Add initial scene */
	game.stackScene(battleScene);

	game.runGame();
	
	return 0;
}
#endif

int main(int argc, char* argv[]) {

	/* Add initial scene */
	game.stackScene(battleScene);

	game.runGame();

	return 0;
}

