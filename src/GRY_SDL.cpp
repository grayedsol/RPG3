/**
 * @file GRY_SDL.cpp
 * @author Grayedsol (grayedsol@gmail.com)
 * @copyright Copyright (c) 2024
 */
#include "GRY_SDL.hpp"
#include "GRY_Log.hpp"
#include "SDL3/SDL.h"
#include "SDL3_image/SDL_image.h"

GRY_SDL::GRY_SDL(int WINDOW_WIDTH, int WINDOW_HEIGHT, bool USE_VSYNC) : 
	WINDOW_WIDTH(WINDOW_WIDTH),
	WINDOW_HEIGHT(WINDOW_HEIGHT),
	USE_VSYNC(USE_VSYNC) {
}

GRY_SDL::~GRY_SDL() { exit(); }

bool GRY_SDL::init() {
	/* Initialize SDL */
	if (SDL_Init(SDL_INIT_VIDEO) < 0) {
		GRY_Log("Could not initialize SDL. Error: %s\n", SDL_GetError());
		return false;
	}

	/* Create window */
	gameWindow = SDL_CreateWindow(
		"SDL Game",
		WINDOW_WIDTH, WINDOW_HEIGHT,
		0);
	
	if (gameWindow == NULL) {
		GRY_Log("Could not create window. Error: %s\n", SDL_GetError());
		return false;
	}

	/* Create renderer */
	if (USE_VSYNC) {
		SDL_PropertiesID properties = SDL_CreateProperties();
		SDL_SetPointerProperty(properties, SDL_PROP_RENDERER_CREATE_WINDOW_POINTER, gameWindow);
		SDL_SetBooleanProperty(properties, SDL_PROP_RENDERER_CREATE_PRESENT_VSYNC_NUMBER, true);
		gameRenderer = SDL_CreateRendererWithProperties(properties);
		SDL_DestroyProperties(properties);
		GRY_Log("[GRY_SDL] Using VSync.\n");
	}
	else { gameRenderer = SDL_CreateRenderer(gameWindow, NULL); }

	if (gameRenderer == NULL) {
		GRY_Log("Could not create renderer. Error: %s\n", SDL_GetError());
		return false;
	}

	/* Initialize renderer draw color */
	SDL_SetRenderDrawColor(gameRenderer, 0x00, 0x00, 0x00, 0x00);
	/* Initialize renderer blend mode */
	SDL_SetRenderDrawBlendMode(gameRenderer, SDL_BLENDMODE_BLEND);

	return true;
}

void GRY_SDL::exit() {	
	/* Free renderer and window. */
	SDL_DestroyRenderer(gameRenderer);
	SDL_DestroyWindow(gameWindow);
	gameRenderer = NULL;
	gameWindow = NULL;

	/* Close SDL subsystems */
	SDL_Quit();
}

SDL_Texture* GRY_SDL::loadTexture(const char* path) {
	SDL_Texture* texture = nullptr;
	SDL_Surface* surface = IMG_Load(path);

	if (!surface) {
		GRY_Log("Could not load texture from file. Error: %s\n", SDL_GetError());
		return texture;
	}

	texture = SDL_CreateTextureFromSurface(gameRenderer, surface);
	if (!texture) {
		GRY_Log("Could not create texture from surface. Error: %s\n", SDL_GetError());
	}

	SDL_DestroySurface(surface);

	SDL_SetTextureScaleMode(texture, SDL_SCALEMODE_NEAREST);
	return texture;
}

SDL_Texture *GRY_SDL::loadTextureIO(const char *data) {
	return nullptr;
}

void GRY_SDL::setWindowFullscreen(bool fullscreen) {
	SDL_SetWindowFullscreen(gameWindow, fullscreen);
}

void GRY_SDL::toggleFullscreen() {
	gameFullscreen = !gameFullscreen;
	SDL_SetWindowFullscreen(gameWindow, gameFullscreen);
}

void GRY_SDL::getWindowSize(int *w, int *h) {
	SDL_GetWindowSize(gameWindow, w, h);	
}

void GRY_SDL::setWindowSize(int w, int h) {
	SDL_SetWindowSize(gameWindow, w, h);
}
