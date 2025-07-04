/**
 * @file TextBoxScene.hpp
 * @author Grayedsol (grayedsol@gmail.com)
 * @brief @copybrief TextBoxScene
 * @copyright Copyright (c) 2025
 */
#pragma once
#include "Scene.hpp"
#include "GRY_Texture.hpp"
#include "SoundResource.hpp"
#include "TextDecisionScene.hpp"
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
public:
	static const unsigned MAX_LINE_LENGTH = 256;
private:
	Scene* parentScene;

	TextDecisionScene decisionScene;

	SoundResource sounds;

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

	char storedLine[MAX_LINE_LENGTH] = { 0 };
	char incomingLine[MAX_LINE_LENGTH] = { 0 };

	int index = 0;
	double timer = 0;

	unsigned audioVoice = 0;
	double audioTimer = 0;

	bool active = false;
	bool speedup = false;

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
	TextBoxScene(GRY_PixelGame *pGame, const char *scenePath, Scene* parentScene);

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

	void setVoice(unsigned soundIndex) { audioVoice = soundIndex; }

	void resetVoice() { audioVoice = 0; }

	void openDecisionBox();

	void closeDecisionBox();

	bool decisionBoxIsOpen();

	bool decisionIsMade();

	unsigned getDecision();

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

	/**
	 * @brief Get the area where text should be typed within the box.
	 * 
	 * @return The text area
	 */
	SDL_Rect getTextArea() const { return textArea; }

	/**
	 * @brief Get the Fontset of the text box.
	 * 
	 * @return `const` reference to the Fontset. 
	 */
	const Fontset& getFont() const { return font; }

	/**
	 * @brief Get a pointer to the GRY_PixelGame.
	 *
	 * @return Pointer to the GRY_PixelGame.
	 */
	GRY_PixelGame* getPixelGame() const { return (GRY_PixelGame*)game; }
};