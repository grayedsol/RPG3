/**
 * @file TextBoxScene.cpp
 * @author Grayedsol (grayedsol@gmail.com)
 * @copyright Copyright (c) 2025
 */
#include "TextBoxScene.hpp"
#include "GRY_JSON.hpp"
#include "GRY_PixelGame.hpp"

static const float BOTTOM_MARGIN = 8.f;

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
	textArea.x = boxTextureArea.x + textArea.w;
	textArea.y = boxTextureArea.y + textArea.h;
	/* Now we can set the actual width and height */
	textArea.w = boxTextureArea.w - (2 * textArea.w);
	textArea.h = boxTextureArea.h - (2 * textArea.h);
}

void TextBoxScene::process() {
	if (readSingleInput() == GCmd::MessageOk) {
		done = !done;
		if (done) {
			delete[] storedLine; storedLine = nullptr;
			delete[] incomingLine; incomingLine = nullptr;
			parentScene->activateControlScheme();
		}
	}
	if (!done) {
		textBoxRenderer.process();
		if (!incomingLine) {
			incomingLine = GRY_copyString("Welcome to the paradise of life itself. Are you struggling to find your ground in such a desolate battleground as this?");
			parseLine(incomingLine);
		}
		textBoxRenderer.printLine(incomingLine);
	}
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
	textArea.w = sceneDoc["marginX"].GetFloat();
	textArea.h = sceneDoc["marginY"].GetFloat();

	return false;
}
