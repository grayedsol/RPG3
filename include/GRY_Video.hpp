/**
 * @file GRY_Video.hpp
 * @author Grayedsol (grayedsol@gmail.com)
 * @brief @copybrief GRY_Video
 * @copyright Copyright (c) 2024
 */
#pragma once

struct SDL_Window;
struct SDL_Renderer;
struct SDL_Texture;

/**
 * @brief Initializes and provides an interface for SDL functionality.
 * 
 * @details
 * Processing SDL_Events for input is not done here, but rather, in InputHandler.
 * 
 * @sa InputHandler
 */
class GRY_Video {
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
	SDL_Window* gameWindow = nullptr;

	/**
	 * @brief Pointer to the SDL_Renderer being used.
	 * 
	 */
	SDL_Renderer* gameRenderer = nullptr;

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
	GRY_Video(int WINDOW_WIDTH, int WINDOW_HEIGHT, bool USE_VSYNC = true);

	/**
	 * @brief Destructor.
	 * 
	 */
	~GRY_Video();

	
	GRY_Video(const GRY_Video&) = delete;
	GRY_Video& operator =(const GRY_Video&) = delete;

	/**
	 * @brief Load a texture from an image file using SDL_image.
	 * 
	 * @param path Path to the image.
	 * @return Pointer to the texture.
	 */
	SDL_Texture* loadTexture(const char* path);

	SDL_Texture* loadTextureIO(const char* data);

	/**
	 * @brief Set the window's fullscreen status.
	 * 
	 * @param fullscreen `true` to set the window to fullscreen, `false` for windowed.
	 */
	void setWindowFullscreen(bool fullscreen);

	/**
	 * @brief Toggle the window's fullscreen status.
	 * 
	 */
	void toggleFullscreen();

	/**
	 * @brief Fill the pointers with the dimensions of the window.
	 * 
	 * @param w Pointer to fill with width of the window. Can be NULL.
	 * @param h Pointer to fill with the height of the window. Can be NULL.
	 */
	void getWindowSize(int* w, int* h);

	/**
	 * @brief Set the size of the window.
	 * 
	 * @param w Width of the window in pixels.
	 * @param h Height of the window in pixels.
	 */
	void setWindowSize(int w, int h);

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
