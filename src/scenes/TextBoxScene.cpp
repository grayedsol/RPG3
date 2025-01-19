/**
 * @file TextBoxScene.cpp
 * @author Grayedsol (grayedsol@gmail.com)
 * @copyright Copyright (c) 2025
 */
#include "TextBoxScene.hpp"
#include "GRY_JSON.hpp"
#include "GRY_PixelGame.hpp"

static const float BOTTOM_MARGIN = 8.f;

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
}

void TextBoxScene::process() {
	if (readSingleInput() == GCmd::MessageOk) {
		done = !done;
		if (done) { parentScene->activateControlScheme(); }
	}
	if (!done) { textBoxRenderer.process(); }
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

	return false;
}
