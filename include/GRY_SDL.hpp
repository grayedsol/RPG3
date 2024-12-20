/**
 * @file GRY_SDL.hpp
 * @author Grayedsol (grayedsol@gmail.com)
 * @brief @copybrief GRY_SDL
 * @copyright Copyright (c) 2024
 */
#pragma once
#include "SDL3/SDL.h"

struct TTF_Font;

/**
 * @brief Initializes and provides an interface for SDL functionality.
 * 
 * @details
 * Processing SDL_Events for input is not done here, but rather, in InputHandler.
 * 
 * @sa InputHandler
 */
class GRY_SDL {
private:
	friend class GRY_Game;

	/**
	 * @brief Window width in pixels.
	 * 
	 */
	int WINDOW_WIDTH;

	/**
	 * @brief Window height in pixels.
	 * 
	 */
	int WINDOW_HEIGHT;

	/**
	 * @brief Status of using VSync.
	 * 
	 */
	const bool USE_VSYNC;

	/**
	 * @brief The game's fullscreen status.
	 * 
	 */
	bool gameFullscreen = false;
	
	/**
	 * @brief Pointer to the SDL_Window being used.
	 * 
	 */
	SDL_Window* gameWindow = NULL;

	/**
	 * @brief Pointer to the SDL_Renderer being used.
	 * 
	 */
	SDL_Renderer* gameRenderer = NULL;

	/**
	 * @brief Closes SDL.
	 * 
	 */
	void exit();

	/**
	 * @brief Initializes SDL.
	 * 
	 * @return `true` if there were no errors during initialization.
	 * @return `false` if there was at least one error.
	 */
	bool init();
public:
	/**
	 * @brief Constructor.
	 * 
	 * @param WINDOW_WIDTH Window width in pixels.
	 * @param WINDOW_HEIGHT Window height in pixels.
	 * @param USE_VSYNC Whether the game will use VSync.
	 */
	GRY_SDL(int WINDOW_WIDTH, int WINDOW_HEIGHT, bool USE_VSYNC = true);

	/**
	 * @brief Destructor.
	 * 
	 */
	~GRY_SDL();

	
	GRY_SDL(const GRY_SDL&) = delete;
	GRY_SDL& operator =(const GRY_SDL&) = delete;

	/**
	 * @brief Load a texture from an image file using SDL_image.
	 * 
	 * @param path Path to the image.
	 * @return Pointer to the texture.
	 */
	SDL_Texture* loadTexture(const char* path);

	SDL_Texture* loadTextureIO(const char* data);

	/**
	 * @brief Load a texture from text.
	 * 
	 * @param text UTF8 encoded text that will be shown in the texture.
	 * @param font Font of the text.
	 * @param color Color of the text.
	 * @return Pointer to the texture.
	 */
	SDL_Texture* loadTextTexture(const char* text, TTF_Font* font, SDL_Color color);

	/**
	 * @brief Load a font from a file.
	 * 
	 * @param fontPath File path to the ttf font.
	 * @param ptSize Point size to use for the font.
	 * @return Pointer to the font.
	 */
	TTF_Font* loadFont(const char* fontPath, int ptSize);

	/**
	 * @brief Set the window's fullscreen status.
	 * 
	 * @param fullscreen `true` to set the window to fullscreen, `false` for windowed.
	 */
	void setWindowFullscreen(bool fullscreen) {
		SDL_SetWindowFullscreen(gameWindow, fullscreen);
	}

	/**
	 * @brief Toggle the window's fullscreen status.
	 * 
	 */
	void toggleFullscreen() {
		SDL_SetWindowFullscreen(gameWindow, !gameFullscreen);
		gameFullscreen = !gameFullscreen;
	}

	/**
	 * @brief Fill the pointers with the dimensions of the window.
	 * 
	 * @param w Pointer to fill with width of the window. Can be NULL.
	 * @param h Pointer to fill with the height of the window. Can be NULL.
	 */
	void getWindowSize(int* w, int* h) { SDL_GetWindowSize(gameWindow, w, h); }

	/**
	 * @brief Set the size of the window.
	 * 
	 * @param w Width of the window in pixels.
	 * @param h Height of the window in pixels.
	 */
	void setWindowSize(int w, int h) { SDL_SetWindowSize(gameWindow, w, h); }

	/**
	 * @brief Get the SDL_Window* being used.
	 * 
	 * @return Reference to the SDL_Window*.
	 */
	SDL_Window* getWindow() { return gameWindow; }

	/**
	 * @brief Get the SDL_Renderer* being used.
	 * 
	 * @return Reference to the SDL_Renderer*.
	 */	
	SDL_Renderer* getRenderer() { return gameRenderer; }
};
