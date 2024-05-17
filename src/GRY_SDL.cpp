/**
 * @file GRY_SDL.cpp
 * @author Grayedsol (grayedsol@gmail.com)
 * @copyright Copyright (c) 2024
 */
#include "GRY_SDL.hpp"

GRY_SDL::GRY_SDL(int WINDOW_WIDTH, int WINDOW_HEIGHT, bool USE_VSYNC) : 
	WINDOW_WIDTH(WINDOW_WIDTH),
	WINDOW_HEIGHT(WINDOW_HEIGHT),
	USE_VSYNC(USE_VSYNC) {}

GRY_SDL::~GRY_SDL() { exit(); }

bool GRY_SDL::init() {
	/* Initialize SDL */
	if (SDL_Init(SDL_INIT_VIDEO) < 0) {
		SDL_Log("Could not initialize SDL. Error: %s", SDL_GetError());
		return false;
	}

	/* Create window */
	gameWindow = SDL_CreateWindow(
		"SDL Game",
		WINDOW_WIDTH, WINDOW_HEIGHT,
		SDL_WINDOW_RESIZABLE);
	
	if (gameWindow == NULL) {
		SDL_Log("Could not create window. Error: %s", SDL_GetError());
		return false;
	}

	/* Create renderer */
	if (USE_VSYNC) {
		gameRenderer = SDL_CreateRenderer( gameWindow, NULL, 
			SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC
		);
		SDL_Log("Using VSync."); }
	else { gameRenderer = SDL_CreateRenderer(gameWindow, NULL, SDL_RENDERER_ACCELERATED); }

	if (gameRenderer == NULL) {
		SDL_Log("Could not create renderer. Error: %s", SDL_GetError());
		return false;
	}

	/* Initialize renderer draw color */
	SDL_SetRenderDrawColor(gameRenderer, 0x00, 0x00, 0x00, 0x00);
	/* Initialize renderer blend mode */
	SDL_SetRenderDrawBlendMode(gameRenderer, SDL_BLENDMODE_BLEND);

	/* Initialize SDL_image */
	int imgFlags = IMG_INIT_PNG;
	if (!(IMG_Init(imgFlags) & imgFlags)) {
		SDL_Log("Could not initialize SDL_Image. Error: %s", SDL_GetError());
		return false;
	}

	return true;
}

void GRY_SDL::exit() {	
	/* Free renderer and window. */
	SDL_DestroyRenderer(gameRenderer);
	SDL_DestroyWindow(gameWindow);
	gameRenderer = NULL;
	gameWindow = NULL;

	/* Close SDL subsystems */
	IMG_Quit();
	SDL_Quit();
}

SDL_Texture* GRY_SDL::loadTexture(const char* path) {
	SDL_Texture* texture = NULL;
	SDL_Surface* surface = IMG_Load(path);

	if (surface == NULL) {
		SDL_Log("Could not load texture from file. Error: %s", IMG_GetError());
		return texture;
	}

	texture = SDL_CreateTextureFromSurface(gameRenderer, surface);
	if (texture == NULL) {
		SDL_Log("Could not render texture from surface. Error: %s", SDL_GetError());
	}

	SDL_DestroySurface(surface);
	return texture;
}
