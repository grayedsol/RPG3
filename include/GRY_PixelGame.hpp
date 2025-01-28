/**
 * @file GRY_PixelGame.hpp
 * @author Grayedsol (grayedsol@gmail.com)
 * @brief @copybrief GRY_PixelGame
 * @copyright Copyright (c) 2024
 */
#pragma once
#include "GRY_Game.hpp"

/**
 * @brief Variation of GRY_Game for games that use pixel art.
 * 
 */
class GRY_PixelGame : public GRY_Game {
private:
	/**
	 * @brief Width of the screen in pixels.
	 * 
	 */
	unsigned int SCREEN_WIDTH_PIXELS;

	/**
	 * @brief Height of the screen in pixels.
	 * 
	 */
	unsigned int SCREEN_HEIGHT_PIXELS;

	/**
	 * @brief Scaling factor for textures. Ideally a whole number.
	 * 
	 */
	float pixelScaling;
public:
	/**
	 * @copydoc GRY_Game::GRY_Game
	 */
	GRY_PixelGame(int WINDOW_WIDTH, int WINDOW_HEIGHT, int MAX_FPS, bool USE_VSYNC = true);

	/**
	 * @copydoc GRY_Game::process
	 * 
	 */
	void process() final override;

	/**
	 * @brief Get the screen width in pixels.
	 * 
	 * @return Width of the screen in pixels.
	 */
	unsigned int getScreenWidthPixels() { return SCREEN_WIDTH_PIXELS; }

	/**
	 * @brief Get the screen height in pixels.
	 * 
	 * @return Height of the screen in pixels.
	 */
	unsigned int getScreenHeightPixels() { return SCREEN_HEIGHT_PIXELS; }

	/**
	 * @brief Get a reference to the pixel scaling factor.
	 * 
	 * @return Reference to the pixel scaling factor.
	 */
	const float& getPixelScalingRef() { return pixelScaling; }
};