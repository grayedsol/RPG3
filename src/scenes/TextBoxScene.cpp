/**
 * @file TextBoxScene.cpp
 * @author Grayedsol (grayedsol@gmail.com)
 * @copyright Copyright (c) 2025
 */
#include "TextBoxScene.hpp"
#include "GRY_JSON.hpp"
#include "GRY_PixelGame.hpp"

static const float BOTTOM_MARGIN = 8.f;
static const double SCROLL_SPEED = 64.0;
static const double TIMER_LENGTH = 0.025;

void TextBoxScene::parseLine(char* line) {
	char* character = line;
	float widthRemaining = textArea.w;
	while (*character) {
		float charWidth = font.getSourceRect(*character - ' ')->w;
		if (*character == ' ' || character == line) {
			float wordWidth = charWidth;
			char* wordStart = character + 1;
			while (*wordStart && *wordStart != ' ') {
				wordWidth += font.getSourceRect(*wordStart - ' ')->w;
				wordStart++;
			}
			if (wordWidth > widthRemaining && wordWidth < textArea.w) {
				*character = '\n';
				widthRemaining = textArea.w - wordWidth + charWidth;
			}
			else {
				character = wordStart;
				widthRemaining -= wordWidth;
				continue;
			}
		}
		character++;
	}
}

void TextBoxScene::setControls() {
	controls.mapCmd(GCmd::MessageOk, VirtualButton::GAME_A);
}

void TextBoxScene::init() {
	setControls();
	
	GRY_PixelGame* pGame = (GRY_PixelGame*)game;
	unsigned screenWidth = pGame->getScreenWidthPixels();
	unsigned screenHeight = pGame->getScreenHeightPixels();

	float textureWidth;
	float textureHeight;
	SDL_GetTextureSize(boxTexture.texture, &textureWidth, &textureHeight);

	float x = ((float)screenWidth - textureWidth) * 0.5f;
	float y = ((float)screenHeight - textureHeight - BOTTOM_MARGIN);

	boxTextureArea = SDL_FRect{ x, y, textureWidth, textureHeight };

	/* Recall that textArea width and height have the margins stored in them right now. */
	textArea.x = (int)boxTextureArea.x + textArea.w;
	textArea.y = (int)boxTextureArea.y + textArea.h;
	/* Now we can set the actual width and height */
	textArea.w = (int)boxTextureArea.w - (2 * textArea.w);
	textArea.h = (int)boxTextureArea.h - (2 * textArea.h);
}

void TextBoxScene::process() {
	if (!active) { return; }

	if (readSingleInput() == GCmd::MessageOk && index != 0) {
		doubleSpeed = true;
	}

	if (*incomingLine && incomingLine[index] == '\0') {
		if (*storedLine && textBoxRenderer.yAfterPrintingLine(storedLine) > 0) {
			textBoxRenderer.beginProcess();
			textBoxRenderer.scrollUp(SCROLL_SPEED * (1 + doubleSpeed), game->getDelta());
			textBoxRenderer.printLine(storedLine, SCROLL_SPEED, game->getDelta());
			textBoxRenderer.printLine(incomingLine, SCROLL_SPEED, game->getDelta());
			textBoxRenderer.endProcess();
			textBoxRenderer.endRender();
			return;
		}
		else {
			strncpy(storedLine, incomingLine, MAX_LINE_LENGTH);
			textBoxRenderer.beginRender(storedLine);
			doubleSpeed = false;
			*incomingLine = 0;
			index = 0;
		}
	}

	textBoxRenderer.beginProcess();
	
	if (*storedLine) {
		textBoxRenderer.printLine(storedLine, SCROLL_SPEED, game->getDelta());
	}

	if (*incomingLine) {
		double speed = SCROLL_SPEED * (1 + doubleSpeed);
		if (textBoxRenderer.printLine(incomingLine, speed, game->getDelta(), index)) {
			timer -= game->getDelta() * (1 + doubleSpeed);
			while (timer <= 0.0) {
				index++;
				timer += TIMER_LENGTH;
			}
		}
	}

	textBoxRenderer.endProcess();
}

bool TextBoxScene::load() {
	if (boxTexture.path && font.path) {
		return boxTexture.load(game) && font.load(game);
	}

	/* Open scene document */
	GRY_JSON::Document sceneDoc;
	GRY_JSON::loadDoc(sceneDoc, scenePath);

	/* Initialize the box texture */
	boxTexture.setPath(sceneDoc["boxTexturePath"].GetString());
	/* Initialize the font texture */
	font.setPath(sceneDoc["fontTexturePath"].GetString());
	/* Store margins in textArea for now */
	textArea.w = sceneDoc["marginX"].GetUint();
	textArea.h = sceneDoc["marginY"].GetUint();
	return false;
}

bool TextBoxScene::isReady() {
	GRY_Assert(active, "[TextBoxScene] You must open the text box before calling isReady()!");
	return readSingleInput() == GCmd::MessageOk && !*incomingLine;
}

void TextBoxScene::open() {
	activateControlScheme();
	active = true;
}

void TextBoxScene::close() {
	*storedLine = 0;
	*incomingLine = 0;
	parentScene->activateControlScheme();
	active = false;
}

void TextBoxScene::printLine(const char *line) {
	GRY_Assert(!*incomingLine, "[TextBoxScene] printLine() called before the textbox was ready.");
	if (*incomingLine) { return; }
	strncpy(incomingLine, line, MAX_LINE_LENGTH);
	incomingLine[MAX_LINE_LENGTH-1] = 0; /**< Ensure null termination */
	parseLine(incomingLine);
}
