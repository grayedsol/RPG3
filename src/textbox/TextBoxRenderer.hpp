/**
 * @file TextBoxRenderer.hpp
 * @author Grayedsol (grayedsol@gmail.com)
 * @brief @copybrief TextBoxRenderer
 * @copyright Copyright (c) 2025
 */
#pragma once
#include "SDL3/SDL.h"

class TextBoxScene;

/**
 * @brief Renders the text box from a TextBoxScene.
 * 
 */
class TextBoxRenderer {
private:
	/**
	 * @brief Associated TextBoxScene class.
	 * 
	 */
	const TextBoxScene* scene;

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

	void printChar(char character, float x, float y);
public:
	/**
	 * @brief Constructor.
	 * 
	 * @param scene Associated TextBoxScene class.
	 */
	TextBoxRenderer(TextBoxScene* scene);

	/**
	 * @brief Render the text box.
	 * 
	 */
	void process();

	void printLine(const char* line);
};