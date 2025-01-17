/**
 * @file TextBoxScene.cpp
 * @author Grayedsol (grayedsol@gmail.com)
 * @copyright Copyright (c) 2025
 */
#include "TextBoxScene.hpp"
#include "GRY_JSON.hpp"

void TextBoxScene::setControls() {
	controls.mapCmd(GCmd::MessageOk, VirtualButton::GAME_A);
}

TextBoxScene::TextBoxScene(GRY_PixelGame *pGame, const char *scenePath) : Scene((GRY_Game *)pGame, scenePath) {
}

void TextBoxScene::process() {
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
