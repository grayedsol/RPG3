/**
 * @file Tileset.cpp
 * @author Grayedsol (grayedsol@gmail.com)
 * @copyright Copyright (c) 2024
 */
#include "Tileset.hpp"
#include "GRY_Game.hpp"
#include "GRY_Tiled.hpp"

bool Tileset::load(GRY_Game* game) {
    if (tileWidth != 0.0f) { return true; }

	/* Open the tileset file */
    GRY_JSON::Document tilesetDoc;
    GRY_JSON::loadDoc(tilesetDoc, path);

    /* Load texture */
    if (!gtexture) {
		const char* imagePath = GRY_Tiled::getProperty(tilesetDoc, "imagePath").GetString();
        gtexture = new GRY_Texture(imagePath);
        return gtexture->load(game);
    }

    /* Read width and height of a single tile */
	tileWidth = tilesetDoc["tilewidth"].GetFloat();
	tileHeight = tilesetDoc["tileheight"].GetFloat();
	assert(tileWidth && tileHeight);

    float texture_width;
	SDL_GetTextureSize(gtexture->texture, &texture_width, NULL);
	/* Width of tileset in number of tiles */
	int tilesetWidth = texture_width / (int)tileWidth;

    /* Get total number of tiles in the tileset */
	TileId tileCount = tilesetDoc["tilecount"].GetUint();

	/* Create texture idx */
	/* Use <= because we need one more at the beginning for 0 index (no tile) */
	for (TileId i = 0; i <= tileCount; i++) {
		textureIdx.push_back(i);
	}

    /* Push empty SDL_FRect for 0 index */
	sourceRects.push_back(SDL_FRect{0,0,0,0});
	/* Load in source rectangles */
	for (int i = 0; i < tileCount; i++) {
		sourceRects.push_back(
			createSourceRect(i, tilesetWidth, (int)tileWidth, (int)tileHeight)
		);
	}

	if (tilesetDoc.HasMember("tiles")) {
		for (auto& tile : tilesetDoc["tiles"].GetArray()) {
			TileId id = tile["id"].GetUint() + 1; /* Add 1 because it's 1-based indexing */
			/* Load animations */
			if (tile.HasMember("animation")) {
				std::vector<TileAnimation::Frame> frames;
				for (auto& frame : tile["animation"].GetArray()) {
					frames.push_back(TileAnimation::Frame {
							frame["duration"].GetDouble() / 1000.0,
							(TileId)(frame["tileid"].GetUint() + 1) /* Add 1 here too */
						}
					);
				}
				tileAnimations.push_back(TileAnimation{frames, 0, frames.at(0).duration, id});
			}
		}
	}

    return false;
}

void Tileset::processAnimations(double delta) {
	for (auto& anim : tileAnimations) {
		anim.timer -= delta;
		if (anim.timer <= 0.0) {
			if (++anim.currentFrame >= anim.frames.size()) { anim.currentFrame = 0; }
			const TileAnimation::Frame& newFrame = anim.frames[anim.currentFrame];
			textureIdx[anim.tile] = newFrame.index;
			anim.timer = newFrame.duration;
		}
	}
}
