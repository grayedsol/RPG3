/**
 * @file TileMapRenderer.cpp
 * @author Grayedsol (grayedsol@gmail.com)
 * @copyright Copyright (c) 2024
 */
#include "TileMapRenderer.hpp"
#include "../scenes/TileMapScene.hpp"
#include "GRY_PixelGame.hpp"
#include "SDL_RectOps.hpp"
#include "SDL3/SDL_render.h"

Tile::MapRenderer::MapRenderer(const MapScene *scene) :
	scene(scene),
	renderer(scene->getGame()->getSDL().getRenderer()),
	tileMap(&scene->getTileMap()),
	entityMap(&scene->getTileEntityMap()),
	pixelScaling(&scene->getPixelGame()->getPixelScalingRef()),
	positions(&scene->getECSReadOnly().getComponentReadOnly<Position2>()),
	sprites(&scene->getECSReadOnly().getComponentReadOnly<ActorSprite>()),
	hitboxes(&scene->getECSReadOnly().getComponentReadOnly<Hitbox>()) {
}

void Tile::MapRenderer::renderTile(const Tileset &tileset, const TileId textureIndex, const SDL_FRect *dstRect) {
	SDL_RenderTexture(renderer, tileset.texture, tileset.getSourceRect(textureIndex), dstRect);
}

void Tile::MapRenderer::renderSprite(ECS::entity e) {
	const Tileset& tileset = entityMap->tilesets[sprites->get(e).tileset];
	SDL_FRect dstRect {
		floorf((positions->get(e)[0] + sprites->get(e).offsetX + offsetX) * *pixelScaling),
		floorf((positions->get(e)[1] + sprites->get(e).offsetY + offsetY) * *pixelScaling),
		tileset.tileWidth * *pixelScaling,
		tileset.tileHeight * *pixelScaling
	};
	SDL_RenderTexture(renderer, tileset.texture, tileset.getSourceRect(sprites->get(e).index), &dstRect);

	#ifndef NDEBUG
	if (!scene->getMapCamera().cameraIsLocked() || scene->getECSReadOnly().getComponentReadOnly<Player>().getEntity(0) != e) { return; }
	SDL_FRect referenceRect {
		floorf(sprites->get(e).offsetX + (scene->getPixelGame()->getScreenWidthPixels() * 0.5f) - (hitboxes->get(e).w * 0.5f)) * *pixelScaling,
		floorf(sprites->get(e).offsetY + (scene->getPixelGame()->getScreenHeightPixels() * 0.5f) - (hitboxes->get(e).h * 0.5f)) * *pixelScaling,
		0, 0
	};
	if (dstRect.x != referenceRect.x || dstRect.y != referenceRect.y) {
		GRY_Log("Floating point error when rendering player!\n");
	}
	#endif
}

/**
 * @details
 * For efficiency, this renderer assumes the map uses only one tileset.
 */
void Tile::MapRenderer::process() {
	GRY_VecTD<uint32_t, 2, void> tileViewport{
		scene->getPixelGame()->getScreenWidthPixels() / scene->getNormalTileSize() + 1,
		scene->getPixelGame()->getScreenHeightPixels() / scene->getNormalTileSize() + 2
	};
	/* Tileset that will be used */
	const Tileset& tileset = tileMap->tileset;
	/* Distance to shift x or y when moving columns/rows in the rendering loop */
	const float shift = scene->getNormalTileSize() * *pixelScaling;

	GRY_Assert(tileMap->tileLayers.size() == entityMap->entityLayers.size(), 
		"[TileMapRenderer] Tile map and Entity map need to have the same number of layers.\n"
	);

	uint32_t startX = std::max(0.f, -offsetX / (float)scene->getNormalTileSize());
	uint32_t startY = std::max(0.f, -offsetY / (float)scene->getNormalTileSize());
	uint32_t endX = std::min(tileMap->width, (uint32_t)(-offsetX / (float)scene->getNormalTileSize()) + tileViewport.x);
	uint32_t endY = (uint32_t)(-offsetY / (float)scene->getNormalTileSize()) + tileViewport.y;
	/* Destination rectangle, defines position and size of rendered tile */
	SDL_FRect dstRect {
		floorf((offsetX + (startX * scene->getNormalTileSize())) * *pixelScaling),
		floorf((offsetY + (startY * scene->getNormalTileSize())) * *pixelScaling),
		tileset.tileWidth * *pixelScaling, tileset.tileHeight * *pixelScaling
	};
	for (int i = 0; i < tileMap->tileLayers.size(); i++) {
		const TileLayer& layer = tileMap->tileLayers[i];
		const EntityLayer& entityLayer = entityMap->entityLayers[i];
		uint32_t numRows = (uint32_t)layer.size() / tileMap->width;

		unsigned entityIndex = 0;
		uint32_t entityRow = 0;
		/* Find the starting entityIndex and entityRow */
		for (; entityIndex < entityLayer.size(); entityIndex++) {
			if (!sprites->contains(entityLayer[entityIndex])) { continue; }
			Hitbox box = hitboxes->get(entityLayer[entityIndex]);
			entityRow = (uint32_t)((box.y + box.h) / tileset.tileHeight);
			if (entityRow >= startY) { break; }
		}

		endY = std::min((uint32_t)tileMap->tileLayers[i].size() / tileMap->width,
						(uint32_t)(-offsetY / (float)scene->getNormalTileSize()) + tileViewport.y);
		/* Render by row */
		for (uint32_t y = startY; y < endY; y++) {
			/* Render row of tiles */
			for (uint32_t x = startX; x < endX; x++) {
				Tile tile = layer[y * tileMap->width + x];
				/* Render when id is nonzero (if it's 0 it has no texture) */
				if (tile.id) { renderTile(tileset, tile.id, &dstRect); }
				/* Increment dstRect x */
				dstRect.x += shift;
			}

			/* Render any entities in the row */
			while (entityRow == y && entityIndex < entityLayer.size()) {
				if (!sprites->contains(entityLayer[entityIndex])) {
					entityIndex++;
					continue;
				}
				renderSprite(entityLayer[entityIndex]);
				entityIndex++;
				if (entityIndex < entityLayer.size()) {
					Hitbox box = hitboxes->get(entityLayer[entityIndex]);
					entityRow = (uint32_t)((box.y + box.h) / tileset.tileHeight);
				}
			}

			/* Reset dstRect x and increment dstRect y */
			dstRect.x = floorf((offsetX + (startX * scene->getNormalTileSize())) * *pixelScaling);
			dstRect.y += shift;
		}
		/* Reset dstRect y */
		dstRect.y = floorf((offsetY + (startY * scene->getNormalTileSize())) * *pixelScaling);
	}
}
