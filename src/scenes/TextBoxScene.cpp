/**
 * @file TextBoxScene.cpp
 * @author Grayedsol (grayedsol@gmail.com)
 * @copyright Copyright (c) 2025
 */
#include "TextBoxScene.hpp"
#include "GRY_JSON.hpp"
#include "GRY_PixelGame.hpp"
#include "SDL3/SDL_render.h"

static const float BOTTOM_MARGIN = 8.f;
static const double BASE_SCROLL_SPEED = 64.0;
static const double SCROLL_SPEED_MULTIPLIER = 2.0;
static const double TIMER_LENGTH = 0.025;

void TextBoxScene::parseLine(char* line) {
	char* character = line;
	float widthRemaining = textArea.w;
	while (*character) {
		float charWidth = font.getSourceRect(*character - ' ')->w;
		if (*character == ' ' || character == line) {
			float wordWidth = charWidth;
			char* word = character + 1;
			while (*word && *word != ' ') {
				wordWidth += font.getSourceRect(*word - ' ')->w;
				word++;
			}
			if (wordWidth > widthRemaining && wordWidth < textArea.w) {
				*character = '\n';
				widthRemaining = textArea.w - wordWidth + charWidth;
			}
			else {
				character = word;
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

	/* Recall that textArea width and height have the margins stored in them right now */
	textArea.x = (int)boxTextureArea.x + textArea.w;
	textArea.y = (int)boxTextureArea.y + textArea.h;
	/* Now we can set the actual width and height */
	textArea.w = (int)boxTextureArea.w - (2 * textArea.w);
	textArea.h = (int)boxTextureArea.h - (2 * textArea.h);

	decisionScene.init();
}

void TextBoxScene::process() {
	if (!active) { return; }

	if (readSingleInput() == GCmd::MessageOk && index != 0) { speedup = true; }
	double speed = BASE_SCROLL_SPEED * (1 + (speedup * SCROLL_SPEED_MULTIPLIER));
	float scrollAmt = speed * game->getDelta();

	textBoxRenderer.beginRender();

	textBoxRenderer.renderLine(storedLine, scrollAmt);

	if (!*incomingLine) { /* Skip to end of function */ }
	/* If incoming line has finished printing */
	else if (incomingLine[index] == 0) {
		/* Scroll up until the text box only shows the incoming line */
		if (textBoxRenderer.getCursorY() > 0) {
			textBoxRenderer.scrollUp(scrollAmt); /**< Does not affect cursor.y this frame */
			textBoxRenderer.renderLine(incomingLine, scrollAmt);
		}
		/* Replace the stored line with the incoming line, reset the incoming line */
		else {
			strncpy(storedLine, incomingLine, MAX_LINE_LENGTH);
			textBoxRenderer.setSpacingFromLine(storedLine);
			textBoxRenderer.renderLine(storedLine, scrollAmt);

			speedup = false;
			*incomingLine = 0;
			index = 0;
		}
	}
	/* Print incoming line. If it successfully printed up to index, increment index on a timer */
	else if (textBoxRenderer.renderLine(incomingLine, scrollAmt, index)) {
		timer -= game->getDelta() * (1 + speedup);
		while (timer <= 0.0) {
			index++;
			timer += TIMER_LENGTH;
		}
	}

	textBoxRenderer.endRender();
	decisionScene.process();
}

bool TextBoxScene::load() {
	if (boxTexture.path && font.path) {
		return boxTexture.load(game) && font.load(game) && decisionScene.load();
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
	return !*incomingLine;
}

void TextBoxScene::open() {
	GRY_Assert(!active, "[TextBoxScene] open() called when the text box was already open!");
	activateControlScheme();
	active = true;
}

void TextBoxScene::close() {
	GRY_Assert(active, "[TextBoxScene] close() called when the text box was not open!");
	*storedLine = 0;
	*incomingLine = 0;
	parentScene->activateControlScheme();
	active = false;
}

void TextBoxScene::openDecisionBox() {
	decisionScene.open();
}

bool TextBoxScene::decisionBoxIsOpen() {
	return decisionScene.isOpen();
}

bool TextBoxScene::decisionIsMade() {
	return decisionScene.decisionIsMade();
}

unsigned TextBoxScene::getDecision() {
	return decisionScene.getDecision();
}

void TextBoxScene::closeDecisionBox() {
	decisionScene.close();
}

void TextBoxScene::printLine(const char *line) {
	GRY_Assert(!*incomingLine, "[TextBoxScene] printLine() called before the textbox was ready.");
	if (*incomingLine) { return; }
	strncpy(incomingLine, line, MAX_LINE_LENGTH);
	incomingLine[MAX_LINE_LENGTH-1] = 0; /**< Ensure null termination */
	parseLine(incomingLine);
}
