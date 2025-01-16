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
	sprites(&scene->getECSReadOnly().getComponentReadOnly<ActorSprite>()),
	hitboxes(&scene->getECSReadOnly().getComponentReadOnly<Hitbox>()) {
}

void TileMapRenderer::renderTile(const Tileset &tileset, const TileId textureIndex, const SDL_FRect *dstRect) {
	SDL_RenderTexture(renderer, tileset.texture, &tileset.sourceRects[textureIndex], dstRect);
}

void TileMapRenderer::renderSprite(ECS::entity e) {
	const Tileset& tileset = entityMap->tilesets[sprites->get(e).tileset];
	SDL_FRect dstRect {
		floorf((positions->get(e)[0] + sprites->get(e).offsetX) * *pixelScaling) + offsetX,
		floorf((positions->get(e)[1] + sprites->get(e).offsetY) * *pixelScaling) + offsetY,
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
	const Tileset& tileset = tileMap->tileset;
	/* Distance to shift x or y when moving columns/rows in the rendering loop */
	const float shift = scene->getNormalTileSize() * *pixelScaling;
	/* Destination rectangle, defines position and size of rendered tile */
	SDL_FRect dstRect { offsetX, offsetY, tileset.tileWidth * *pixelScaling, tileset.tileHeight * *pixelScaling };

	GRY_Assert(tileMap->tileLayers.size() == entityMap->entityLayers.size(), 
		"[TileMapRenderer] Tile map and Entity map need to have the same number of layers.\n"
	);

	for (int i = 0; i < tileMap->tileLayers.size(); i++) {
		const TileLayer& layer = tileMap->tileLayers[i];
		const EntityLayer& entityLayer = entityMap->entityLayers[i];
		uint32_t numRows = (uint32_t)layer.size() / tileMap->width;

		unsigned entityIndex = 0;
		uint32_t entityRow = 0;
		if (!entityLayer.empty()) {
			Hitbox box = hitboxes->get(entityLayer[entityIndex]);
			entityRow = (uint32_t)((box.y + box.h) / tileset.tileHeight);
		}
		/* Render by row */
		for (uint32_t y = 0; y < numRows; y++) {
			/* Render row of tiles */
			for (uint32_t x = 0; x < tileMap->width; x++) {
				Tile tile = layer[y * tileMap->width + x];
				/* Render when id is nonzero (if it's 0 it has no texture) */
				if (tile.id) { renderTile(tileset, tile.id, &dstRect); }
				/* Increment dstRect x */
				dstRect.x += shift;
			}
			/* Reset dstRect x and increment dstRect y */
			dstRect.x =  offsetX;
			dstRect.y += shift;

			/* Render any entities in the row */
			while (entityRow == y && entityIndex < entityLayer.size()) {
				renderSprite(entityLayer[entityIndex]);
				entityIndex++;
				if (entityIndex < entityLayer.size()) {
					Hitbox box = hitboxes->get(entityLayer[entityIndex]);
					entityRow = (uint32_t)((box.y + box.h) / tileset.tileHeight);
				}
			}
		}
		/* Reset dstRect y */
		dstRect.y = offsetY;
	}
}
