/**
 * @file Tileset.cpp
 * @author Grayedsol (grayedsol@gmail.com)
 * @copyright Copyright (c) 2024
 */
#include "Tileset.hpp"
#include "GRY_Game.hpp"
#include "GRY_Tiled.hpp"
#include "SDL3/SDL_render.h"

Tile::Tileset::~Tileset() {
	SDL_DestroyTexture(texture);
	texture = nullptr;
}

bool Tile::Tileset::load(GRY_Game* game) {
    if (tileWidth != 0.0f) { return true; }

	/* Open the tileset file */
    GRY_JSON::Document tilesetDoc;
    GRY_JSON::loadDoc(tilesetDoc, path);

	/* Load texture */
	if (!texture) {
		const char* imagePath = GRY_Tiled::getProperty(tilesetDoc, "imagePath").GetString();
		texture = game->getSDL().loadTexture(imagePath);
		return false;
	}

    /* Read width and height of a single tile */
	tileWidth = tilesetDoc["tilewidth"].GetFloat();
	tileHeight = tilesetDoc["tileheight"].GetFloat();
	GRY_Assert((tileWidth > 0) && (tileHeight > 0), "[Tileset] Invalid tileWidth or tileHeight.");

    float texture_width;
	SDL_GetTextureSize(texture, &texture_width, NULL);
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
				std::vector<Animation::Frame> frames;
				for (auto& frame : tile["animation"].GetArray()) {
					frames.push_back(Animation::Frame {
							frame["duration"].GetDouble() / 1000.0,
							(TileId)(frame["tileid"].GetUint() + 1) /* Add 1 here too */
						}
					);
				}
				tileAnimations.push_back(Animation{frames, 0, frames.at(0).duration, id});
			}
			/* Change textureIdx */
			/* TODO: Find a way to make this better until Tiled finally adds arrays >:( */
			if (GRY_Tiled::searchProperty(tile, "textureIdx")) {
				textureIdx.at(GRY_Tiled::getProperty(tile, "textureIdx").GetUint()) = id;
			}
			if (GRY_Tiled::searchProperty(tile, "textureIdx2")) {
				textureIdx.at(GRY_Tiled::getProperty(tile, "textureIdx2").GetUint()) = id;
			}
			if (GRY_Tiled::searchProperty(tile, "textureIdx3")) {
				textureIdx.at(GRY_Tiled::getProperty(tile, "textureIdx3").GetUint()) = id;
			}
		}
	}

    return false;
}

void Tile::Tileset::processAnimations(double delta) {
	for (auto& anim : tileAnimations) {
		anim.timer -= delta;
		if (anim.timer > 0.0) { continue; }
		
		if (++anim.currentFrame >= anim.frames.size()) { anim.currentFrame = 0; }
		const Animation::Frame& newFrame = anim.frames[anim.currentFrame];
		textureIdx[anim.tile] = newFrame.index;
		anim.timer = newFrame.duration;
	}
}
