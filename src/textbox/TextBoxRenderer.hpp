/**
 * @file TextBoxRenderer.hpp
 * @author Grayedsol (grayedsol@gmail.com)
 * @brief @copybrief TextBoxRenderer
 * @copyright Copyright (c) 2025
 */
#pragma once

struct SDL_Renderer;
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

	/**
	 * @brief Render a line of text to the text box.
	 * 
	 * @param line Line of text to render
	 * @param scrollAmt Distance to scroll if there is not enough space
	 * @param index Character index to print to, or -1 for the whole line
	 * @return `true` if the line was rendered up to `index`,
	 * @return `false` if there was not enough room or the line was empty
	 */
	bool renderLine(const char* line, float scrollAmt, int index = -1);

	/**
	 * @brief Set vertical spacing based on a line so it can be completely printed.
	 * 
	 * @param line Line to set spacing for
	 */
	void setSpacingFromLine(const char* line);

	/**
	 * @brief Move the vertical spacing up by an amount.
	 * 
	 * @param scrollAmt Distance to scroll
	 */
	void scrollUp(float scrollAmt) { yStart -= scrollAmt; }

	/**
	 * @brief Get the current y position of the cursor.
	 * 
	 * @return y position of the cursor
	 */
	float getCursorY() { return cursor.y; }
};