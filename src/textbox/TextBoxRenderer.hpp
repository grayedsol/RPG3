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

	struct {
		float x = 0;
		float y = 0;
	} cursor;

	float yStart = 0;
public:
	/**
	 * @brief Constructor.
	 * 
	 * @param scene Associated TextBoxScene class.
	 */
	TextBoxRenderer(TextBoxScene* scene);

	/**
	 * @brief Set the viewport and prepare for rendering text.
	 * 
	 */
	void beginRender();

	/**
	 * @brief Set the viewport back to normal.
	 * 
	 */
	void endRender();

	bool printLine(const char* line, double scrollAmt, int index = -1);

	void setSpacingFromLine(const char* storedLine);

	void scrollUp(double scrollAmt) { yStart -= scrollAmt; }

	float getCursorY() { return cursor.y; }
};