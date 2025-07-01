/**
 * @file GRY_PixelGame.cpp
 * @author Grayedsol (grayedsol@gmail.com)
 * @copyright Copyright (c) 2024
 */
#include "GRY_PixelGame.hpp"
#include "GRY_JSON.hpp"

static const char* PIXEL_GAME_CONFIG_PATH = "config/pixelGameConfig.json";

GRY_PixelGame::GRY_PixelGame(int WINDOW_WIDTH, int WINDOW_HEIGHT, int MAX_FPS, bool USE_VSYNC) : 
	GRY_Game(WINDOW_WIDTH, WINDOW_HEIGHT, MAX_FPS, USE_VSYNC) {
	GRY_JSON::Document doc;
	GRY_JSON::loadDoc(doc, PIXEL_GAME_CONFIG_PATH);

	SCREEN_WIDTH_PIXELS = doc["SCREEN_WIDTH_PIXELS"].GetUint();
	SCREEN_HEIGHT_PIXELS = doc["SCREEN_HEIGHT_PIXELS"].GetUint();

	int w;
	video.getWindowSize(&w, NULL);
	pixelScaling = w / (float)SCREEN_WIDTH_PIXELS;
	pixelScaling = SDL_ceilf(pixelScaling);
}

/**
 * @details
 * Updates the pixel scaling of the game based on the screen size.
 * Toggles the debug screen if SELECT is pressed while START is being pressed.
 */
void GRY_PixelGame::process() {
	int w;
	video.getWindowSize(&w, NULL);
	pixelScaling = w / (float)SCREEN_WIDTH_PIXELS;
	pixelScaling = SDL_ceilf(pixelScaling);

	if (input.isPressingVButton(GAME_START) && input.getSingleInputVButton() == GAME_SELECT) {
		imguiDebug.toggle();
	}
}
