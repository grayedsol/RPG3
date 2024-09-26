/**
 * @file TileMapRenderer.cpp
 * @author Grayedsol (grayedsol@gmail.com)
 * @copyright Copyright (c) 2024
 */
#include "TileMapRenderer.hpp"
#include "../scenes/TileMapScene.hpp"

TileMapRenderer::TileMapRenderer(const TileMapScene* scene) :
	scene(scene),
	renderer(scene->getGame()->getSDL().getRenderer()),
	pixelScaling(scene->getPixelGame()->getPixelScalingRef()) {
}

void TileMapRenderer::renderTile(const Tileset &tileset, const TileId textureIndex, const SDL_FRect *dstRect) {
	SDL_RenderTexture(renderer, tileset.gtexture->texture, &tileset.sourceRects[textureIndex], dstRect);
}

/**
 * @details
 * For efficiency, this renderer assumes the map uses only one tileset.
 */
void TileMapRenderer::process(const TileMap& map) {
	/* Tileset that will be used */
	const Tileset& tileset = map.tilesets[0];
	/* Distance to shift x or y when moving columns/rows in the rendering loop */
	const float shift = scene->getNormalTileSize() * pixelScaling;
	/* Destination rectangle, defines position and size of rendered tile */
	SDL_FRect dstRect { 0, 0, tileset.tileWidth * pixelScaling, tileset.tileHeight * pixelScaling };
	
	for (auto& layer : map.tileLayers) {
		uint32_t numRows = (uint32_t)layer.size() / map.width;
		for (uint32_t y = 0; y < numRows; y++) {
			/* Render row of tiles */
			for (uint32_t x = 0; x < map.width; x++) {
				Tile tile = layer[y * map.width + x];
				/* Only render when id is nonzero (if it's 0 it has no texture) */
				if (tile.id) { renderTile(tileset, tile.id, &dstRect); }
				/* Increment x */
				dstRect.x += shift;
			}
			/* Reset x and increment y */
			dstRect.x = 0;
			dstRect.y += shift;
		}
		/* Reset y */
		dstRect.y = 0;
	}
}
