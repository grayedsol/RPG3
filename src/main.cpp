/**
 * @file main.cpp
 * @author Grayedsol (grayedsol@gmail.com)
 * @copyright Copyright (c) 2024
 */
#include <stdio.h>
#include "GRY_Game.hpp"
#include "scenes/TilesetScene.hpp"

int WINDOW_WIDTH = 960;
int WINDOW_HEIGHT = 540;
int TARGET_FPS = 120;
bool USE_VSYNC = true;

const char* scenePath = "assets/tilesetscene/scene.json";

#ifdef _WIN32
#define NOMINMAX
#include <Windows.h>
#include <libloaderapi.h>
_Use_decl_annotations_ int WINAPI WinMain(
	HINSTANCE hInstance,
	HINSTANCE hPrevInstance,
	LPSTR lpCmdLine,
	int nCmdShow) {

	/* Initialize game */
    GRY_Game* grayGame = new GRY_Game(WINDOW_WIDTH, WINDOW_HEIGHT, TARGET_FPS, USE_VSYNC);
	
	/* Add a scene */
	grayGame->stackScene(new TilesetScene(grayGame, scenePath));

	grayGame->runGame();

	delete grayGame;

	return 0;
}
#endif

int main(int argc, char* argv[]) {

	/* Initialize game */
	GRY_Game* grayGame = new GRY_Game(WINDOW_WIDTH, WINDOW_HEIGHT, TARGET_FPS, USE_VSYNC);
	
	/* Add a scene */
	grayGame->stackScene(new TilesetScene(grayGame, scenePath));

	grayGame->runGame();

	delete grayGame;

	return 0;
}