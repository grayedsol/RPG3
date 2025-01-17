/**
 * @file TextBoxRenderer.hpp
 * @author Grayedsol (grayedsol@gmail.com)
 * @brief @copybrief TextBoxRenderer
 * @copyright Copyright (c) 2025
 */
#pragma once
#include "SDL3/SDL.h"

class TextBoxScene;

class TextBoxRenderer {
private:
	/**
	 * @brief Associated TextBoxScene class.
	 * 
	 */
	const TextBoxScene* scene;

	/**
	 * @brief Destination rectangle for the text box.
	 * 
	 */
	SDL_FRect dstRect;

	/**
	 * @brief Pointer to the renderer.
	 * 
	 */
	SDL_Renderer* renderer;

	/**
	 * @brief Scaling factor for the box texture.
	 * 
	 */
	const float* pixelScaling;
public:
	/**
	 * @brief Constructor.
	 * 
	 * @param scene Associated TextBoxScene class.
	 */
	TextBoxRenderer(TextBoxScene* scene);

	/**
	 * @brief Initializes the renderer. Must be called once before using `process`.
	 * 
	 */
	void init();

	/**
	 * @brief Render the text box.
	 * 
	 */
	void process();
};