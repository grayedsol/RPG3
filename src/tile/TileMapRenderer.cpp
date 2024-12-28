/**
 * @file TileMapRenderer.cpp
 * @author Grayedsol (grayedsol@gmail.com)
 * @copyright Copyright (c) 2024
 */
#include "TileMapRenderer.hpp"
#include "../scenes/TileMapScene.hpp"

TileMapRenderer::TileMapRenderer(const TileMapScene *scene) :
	scene(scene),
	renderer(scene->getGame()->getSDL().getRenderer()),
	tileMap(&scene->getTileMap()),
	entityMap(&scene->getTileEntityMap()),
	pixelScaling(&scene->getPixelGame()->getPixelScalingRef()),
	positions(&scene->getECSReadOnly().getComponentReadOnly<Position2>()),
	sprites(&scene->getECSReadOnly().getComponentReadOnly<ActorSprite>()) {
}

void TileMapRenderer::renderTile(const Tileset &tileset, const TileId textureIndex, const SDL_FRect *dstRect) {
	SDL_RenderTexture(renderer, tileset.texture, &tileset.sourceRects[textureIndex], dstRect);
}

void TileMapRenderer::renderSprite(ECS::entity e) {
	const Tileset& tileset = entityMap->tilesets[sprites->get(e).tileset];
	SDL_FRect dstRect {
		floorf((positions->get(e)[0] + sprites->get(e).offsetX) * *pixelScaling),
		floorf((positions->get(e)[1] + sprites->get(e).offsetY) * *pixelScaling),
		tileset.tileWidth * *pixelScaling,
		tileset.tileHeight * *pixelScaling
	};
	SDL_RenderTexture(renderer, tileset.texture, tileset.getSourceRect(sprites->get(e).index), &dstRect);
}

/**
 * @details
 * For efficiency, this renderer assumes the map uses only one tileset.
 */
void TileMapRenderer::process() {
	/* Tileset that will be used */
	const Tileset& tileset = tileMap->tilesets[0];
	/* Distance to shift x or y when moving columns/rows in the rendering loop */
	const float shift = scene->getNormalTileSize() * *pixelScaling;
	/* Destination rectangle, defines position and size of rendered tile */
	SDL_FRect dstRect { 0, 0, tileset.tileWidth * *pixelScaling, tileset.tileHeight * *pixelScaling };

	GRY_Assert(tileMap->tileLayers.size() == entityMap->entityLayers.size(), 
		"[TileMapRenderer] Tile map and Entity map need to have the same number of layers.\n"
	);

	for (int i = 0; i < tileMap->tileLayers.size(); i++) {
		const TileLayer& layer = tileMap->tileLayers[i];
		const EntityLayer& entityLayer = entityMap->entityLayers[i];
		uint32_t numRows = (uint32_t)layer.size() / tileMap->width;

		unsigned entityIndex = 0;
		uint32_t entityRow = entityLayer.empty() ? 0 : (uint32_t)(positions->get(entityLayer[entityIndex])[1] / tileset.tileHeight);
		/* Render by row */
		for (uint32_t y = 0; y < numRows; y++) {
			/* Render any entities in the row */
			while (entityRow == y && entityIndex < entityLayer.size()) {
				renderSprite(entityLayer[entityIndex]);
				entityIndex++;
				if (entityIndex < entityLayer.size()) {
					entityRow = (uint32_t)(positions->get(entityLayer[entityIndex])[1] / tileset.tileHeight);
				}
			}
			/* Render row of tiles */
			for (uint32_t x = 0; x < tileMap->width; x++) {
				Tile tile = layer[y * tileMap->width + x];
				/* Render when id is nonzero (if it's 0 it has no texture) */
				if (tile.id) { renderTile(tileset, tile.id, &dstRect); }
				/* Else skip by the number of empty tiles in the row after the current one */
				// else {
				// 	x += tile.custom;
				// 	dstRect.x += shift * tile.custom;
				// }
				/* Increment dstRect x */
				dstRect.x += shift;
			}
			/* Reset dstRect x and increment dstRect y */
			dstRect.x = 0;
			dstRect.y += shift;
		}
		/* Reset dstRect y */
		dstRect.y = 0;
	}
}
