#include "BattleSpriteResource.hpp"
#include "GRY_Game.hpp"
#include "GRY_Tiled.hpp"
#include "SDL3/SDL_render.h"

Battle::SpriteResource::~SpriteResource() {
	SDL_DestroyTexture(texture);
	texture = nullptr;
}

bool Battle::SpriteResource::load(GRY_Game *game) {
	if (frameWidth != 0.f) { return true; }

	/* Open the tileset file */
    GRY_JSON::Document tilesetDoc;
    GRY_JSON::loadDoc(tilesetDoc, path);

	/* Load texture */
	if (!texture) {
		const char* imagePath = GRY_Tiled::getProperty(tilesetDoc, "imagePath").GetString();
		texture = game->getVideo().loadTexture(imagePath);
		return false;
	}

	frameWidth = tilesetDoc["tilewidth"].GetFloat();
	frameHeight = tilesetDoc["tileheight"].GetFloat();
	GRY_Assert((frameWidth > 0) && (frameHeight > 0), "[SpriteResource] Invalid tileWidth or tileHeight in tileset file.");

    float texture_width;
	SDL_GetTextureSize(texture, &texture_width, NULL);
	/* Width of tileset in number of frames */
	int tilesetWidth = texture_width / (int)frameWidth;

    /* Get total number of frames in the tileset */
	uint32_t frameCount = tilesetDoc["tilecount"].GetUint();

	/* Load in source rectangles */
	for (int i = 0; i < frameCount; i++) {
		sourceRects.push_back(
			createSourceRect(i, tilesetWidth, (int)frameWidth, (int)frameHeight)
		);
	}

	return false;
}
