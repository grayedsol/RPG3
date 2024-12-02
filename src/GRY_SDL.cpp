/**
 * @file GRY_SDL.cpp
 * @author Grayedsol (grayedsol@gmail.com)
 * @copyright Copyright (c) 2024
 */
#include "GRY_SDL.hpp"
#include "GRY_Log.hpp"
#include "SDL3_image/SDL_image.h"
#include "SDL3_ttf/SDL_ttf.h"

GRY_SDL::GRY_SDL(int WINDOW_WIDTH, int WINDOW_HEIGHT, bool USE_VSYNC) : 
	WINDOW_WIDTH(WINDOW_WIDTH),
	WINDOW_HEIGHT(WINDOW_HEIGHT),
	USE_VSYNC(USE_VSYNC) {}

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
		SDL_WINDOW_RESIZABLE);
	
	if (gameWindow == NULL) {
		GRY_Log("Could not create window. Error: %s\n", SDL_GetError());
		return false;
	}

	/* Create renderer */
	if (USE_VSYNC) {
		SDL_PropertiesID properties = SDL_CreateProperties();
		SDL_SetPointerProperty(properties, SDL_PROP_RENDERER_CREATE_WINDOW_POINTER, gameWindow);
		SDL_SetBooleanProperty(properties, SDL_PROP_RENDERER_CREATE_PRESENT_VSYNC_NUMBER, true);
		// SDL_SetStringProperty(properties, SDL_PROP_RENDERER_CREATE_NAME_STRING, "OpenGL");
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

	/* Initialize SDL_image */
	int imgFlags = IMG_INIT_PNG;
	if (!(IMG_Init(imgFlags) & imgFlags)) {
		GRY_Log("Could not initialize SDL_image. Error: %s\n", SDL_GetError());
		return false;
	}

	/* Initialize SDL_ttf */
	if (TTF_Init() == -1) {
		GRY_Log("Could not initialize SDL_ttf. Error: %s\n", SDL_GetError());
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
	TTF_Quit();
	IMG_Quit();
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

/**
 * @details 
 * The text will wrap to a new line on newline characters in `text`.
 */
SDL_Texture *GRY_SDL::loadTextTexture(const char *text, TTF_Font* font, SDL_Color color) {
	SDL_Texture* texture = nullptr;
	SDL_Surface* surface = TTF_RenderText_Solid_Wrapped(font, text, 0, color, 0);

	if (!surface) {
		GRY_Log("Could not create text surface. Error: %s\n", SDL_GetError());
		return texture;
	}

	texture = SDL_CreateTextureFromSurface(gameRenderer, surface);
	if (!texture) {
		GRY_Log("Could not create texture from surface. Error: %s\n", SDL_GetError());
	}

	SDL_DestroySurface(surface);
	return texture;
}

TTF_Font* GRY_SDL::loadFont(const char *fontPath, int ptSize) {
    TTF_Font* font = TTF_OpenFont(fontPath, ptSize);

	if (!font) {
		GRY_Log("Could not load font. Error: %s\n", SDL_GetError());
	}

	return font;
}
