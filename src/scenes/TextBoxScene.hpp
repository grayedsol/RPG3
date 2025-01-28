/**
 * @file TextBoxScene.hpp
 * @author Grayedsol (grayedsol@gmail.com)
 * @brief @copybrief TextBoxScene
 * @copyright Copyright (c) 2025
 */
#pragma once
#include "Scene.hpp"
#include "GRY_Texture.hpp"
#include "../textbox/Fontset.hpp"
#include "../textbox/TextBoxRenderer.hpp"

class GRY_PixelGame;

/**
 * @brief Displays an interactive text box at the bottom of the screen.
 * 
 * Controls:
 * GAME_A: Proceed
 */
class TextBoxScene : public Scene {
private:
	Scene* parentScene;
	/**
	 * @brief Texture of the message box.
	 * 
	 */
	GRY_Texture boxTexture;

	/**
	 * @brief Destination rectangle for the text box.
	 * 
	 */
	SDL_FRect boxTextureArea;

	/**
	 * @brief Area within the box that text is rendered.
	 * 
	 */
	SDL_Rect textArea;

	/**
	 * @brief Texture of the message font.
	 * 
	 */
	Fontset font;

	/**
	 * @brief Renders the text box.
	 * 
	 */
	TextBoxRenderer textBoxRenderer;

	static const unsigned MAX_LINE_LENGTH = 256;
	char storedLine[MAX_LINE_LENGTH] = { 0 };
	char incomingLine[MAX_LINE_LENGTH] = { 0 };

	int index = 0;
	double timer = 0;

	bool active = false;

	void parseLine(char* line);

	/**
	 * @copybrief Scene::setControls
	 *
	 */
	void setControls() final;
public:
	/**
	 * @brief Constructor.
	 *
	 * @param game Associated game class
	 * @param scenePath File path to the scene data
	 * @param parentScene Scene to activate when done
	 */
	TextBoxScene(GRY_PixelGame *pGame, const char *scenePath, Scene* parentScene) :
		Scene((GRY_Game *)pGame, scenePath),
		parentScene(parentScene),
		textBoxRenderer(this) {
	}

	/**
	 * @brief Initializes the scene.
	 *
	 */
	void init() final;

	/**
	 * @copydoc Scene::process
	 */
	void process() final;

	/**
	 * @copydoc Scene::load
	 */
	bool load() final;

	/**
	 * @brief Whether the box is ready to accept a new line of dialogue.
	 * 
	 * @return `true` if the the box is ready,
	 * @return `false` otherwise.
	 */
	bool isReady();

	/**
	 * @brief Whether the text box is open or not.
	 * 
	 * @return `true` if the text box is open,
	 * @return `false` if not.
	 */
	bool isOpen() { return active; }

	/**
	 * @brief Opens and activates the text box.
	 * 
	 */
	void open();

	/**
	 * @brief Closes the textbox and activates the parent scene.
	 * 
	 */
	void close();

	/**
	 * @brief Print a line to the text box.
	 * 
	 * @param line Line to print to the text box
	 */
	void printLine(const char* line);

	/**
	 * @brief Get the text box texture.
	 * 
	 * @return The text box texture
	 */
	SDL_Texture* getBoxTexture() const { return boxTexture.texture; }

	/**
	 * @brief Get the text box texture area.
	 * 
	 * @return The text box texture area
	 */
	SDL_FRect getBoxTextureArea() const { return boxTextureArea; }

	SDL_Rect getTextArea() const { return textArea; }

	const Fontset& getFont() const { return font; }

	/**
	 * @brief Get a pointer to the GRY_PixelGame.
	 *
	 * @return Pointer to the GRY_PixelGame.
	 */
	GRY_PixelGame* getPixelGame() const { return (GRY_PixelGame*)game; }

	const char* getStoredLine() const { return storedLine; }
	const char* getIncomingLine() const { return incomingLine; }
};