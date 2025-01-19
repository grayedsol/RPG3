/**
 * @file Fontset.cpp
 * @author Grayedsol (grayedsol@gmail.com)
 * @copyright Copyright (c) 2025
 */
#include "Fontset.hpp"
#include "GRY_JSON.hpp"
#include "GRY_Game.hpp"

static SDL_FRect createSourceRect(int textureIndex, int textureWidth, int emWidth, int charWidth, int charHeight);

bool Fontset::load(GRY_Game *game) {
	if (charHeight != 0.f) { return true; }

	/* Open the font file */
	GRY_JSON::Document fontDoc;
	GRY_JSON::loadDoc(fontDoc, path);

	/* Load texture */
	if (!texture) {
		texture = game->getSDL().loadTexture(fontDoc["texture"].GetString());
		return false;
	}

	/* Read charHeight and emWidth */
	charHeight = fontDoc["charHeight"].GetFloat();
	emWidth = fontDoc["emWidth"].GetFloat();
	GRY_Assert((charHeight > 0) && (emWidth > 0), "[Fontset] Invalid charHeight or emWidth.");

	float textureWidth;
	float textureHeight;
	SDL_GetTextureSize(texture, &textureWidth, &textureHeight);

	/* Calculate width and height of the fontset, in number of characters */
	int fontsetWidth = (int)(textureWidth / emWidth);
	int fontsetHeight = (int)(textureHeight / charHeight);

	int charCount = fontsetWidth * fontsetHeight;

	GRY_Assert(fontDoc["widths"].GetArray().Size() == charCount,
		"[Fontset] Mismatch between character count (%d) and size of widths array (%d).",
		charCount, fontDoc["widths"].GetArray().Size()
	);

	for (int i = 0; i < charCount; i++) {
		int charWidth = fontDoc["widths"].GetArray()[i].GetInt();
		sourceRects.push_back(createSourceRect(i, fontsetWidth, emWidth, charWidth, charHeight));
	}

	return false;
}

SDL_FRect createSourceRect(int textureIndex, int textureWidth, int emWidth, int charWidth, int charHeight) {
	return SDL_FRect{
		(float)(textureIndex % textureWidth) * emWidth,
		(float)(textureIndex / textureWidth) * charHeight,
		(float)charWidth,
		(float)charHeight
	};
}
