/**
 * @file main.cpp
 * @author Grayedsol (grayedsol@gmail.com)
 * @copyright Copyright (c) 2024
 */
#include <stdio.h>
#include "GRY_PixelGame.hpp"
#include "scenes/TilesetScene.hpp"
#include "scenes/TileMapScene.hpp"

int WINDOW_WIDTH = 960;
int WINDOW_HEIGHT = 540;
int TARGET_FPS = 120;
bool USE_VSYNC = true;

const char* scenePath = "assets/tilemapscene/scene.json";

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

	/* Initialize game */
    //GRY_Game* game = new GRY_Game(WINDOW_WIDTH, WINDOW_HEIGHT, TARGET_FPS, USE_VSYNC);
	GRY_PixelGame* game = new GRY_PixelGame(WINDOW_WIDTH, WINDOW_HEIGHT, TARGET_FPS, USE_VSYNC);
	
	/* Add a scene */
	game->stackScene(new TileMapScene(game, scenePath));

	game->runGame();

	delete game;

	return 0;
}
#endif

int main(int argc, char* argv[]) {

	/* Initialize game */
    //GRY_Game* game = new GRY_Game(WINDOW_WIDTH, WINDOW_HEIGHT, TARGET_FPS, USE_VSYNC);
	GRY_PixelGame* game = new GRY_PixelGame(WINDOW_WIDTH, WINDOW_HEIGHT, TARGET_FPS, USE_VSYNC);
	
	/* Add a scene */
	game->stackScene(new TileMapScene(game, scenePath));

	game->runGame();

	delete game;

	return 0;
}

