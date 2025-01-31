/**
 * @file TilesetScene.cpp
 * @author Grayedsol (grayedsol@gmail.com)
 * @copyright Copyright (c) 2024
 */
#include "TilesetScene.hpp"
#include "GRY_Game.hpp"
#include "GRY_JSON.hpp"
#include "SDL3/SDL_render.h"
#include "../transitions/FadeToBlack.hpp"
#ifndef NDEBUG
#include "imgui.h"
#endif

/**
 * @details
 * Controls:
 * GAME_A: Switch to a new TilesetScene with transition
 * GAME_B: Quit
 * GAME_UP: Highlight tile collision boxes
*/
void TilesetScene::setControls() {
	controls.mapCmd(GCmd::GameMenu, VirtualButton::GAME_A);
	controls.mapCmd(GCmd::GameUp, VirtualButton::GAME_UP);
	controls.mapCmd(GCmd::GameQuit, VirtualButton::GAME_B);
}

TilesetScene::TilesetScene(GRY_Game *game, const char *tilesetPath) :
	Scene(game, tilesetPath),
	renderer(game->getSDL().getRenderer()) {
}

void TilesetScene::process() {
	/* Determines if collision will be highlighted */
	bool highlightCollisions = false;

	/* Check for pressed inputs */
	if (isPressing(GCmd::GameUp)) {
		highlightCollisions = true;
	}
	/* Check for inputs from this frame only */
	switch (readSingleInput()) {
		case GCmd::GameMenu:
			game->switchScene(new TilesetScene(game, scenePath), new FadeToBlack(game));
			break;
		case GCmd::GameQuit:
			game->quit();
			break;
		default:
			break;
	}

	/* Process tileset animations */
	tileset.processAnimations(game->getDelta());

	/* Tileset width in pixels */
	float tilesetWidth;
	SDL_GetTextureSize(tileset.texture, &tilesetWidth, NULL);
	/* Tileset width in tiles */
	int tilesetWidthTiles = tilesetWidth / tileset.tileWidth;
	/* Destination rectangle for rendering */
	SDL_FRect dstRect{ 0, 0, tileset.tileWidth, tileset.tileHeight };
	/* Display the tileset with a spacing between tiles. Tilesets use 1-based indexing. */
	for (Tile::TileId i = 1; i < tileset.sourceRects.size(); i++) {
		const SDL_FRect* srcRect = tileset.getSourceRect(i);
		SDL_RenderTexture(renderer, tileset.texture, srcRect, &dstRect);

		/* Highlight collisions */
		if (highlightCollisions && collisions.collisions.contains(i)) {
			SDL_FRect collisionRect = *collisions.getCollision(i);
			collisionRect.x += dstRect.x;
			collisionRect.y += dstRect.y;
			SDL_SetRenderDrawColor(renderer, 128, 0, 0, 128);
			SDL_RenderFillRect(renderer, &collisionRect);
			SDL_SetRenderDrawColor(renderer, 0, 0, 0, 0);
		}

		dstRect.x += tileset.tileWidth + spacing;
		if (dstRect.x >= (spacing + tileset.tileWidth) * tilesetWidthTiles) {
			dstRect.x = 0;
			dstRect.y += tileset.tileHeight + spacing;
		}
	}
	#ifndef NDEBUG
	size_t numTiles = tileset.sourceRects.size() - 1;
	ImGui::Begin("Tileset Scene");
	ImGui::Text("Number of tiles: %i", numTiles);
	ImGui::End();
	#endif
}

bool TilesetScene::load() {
	if (tileset.path && collisions.path) {
		return
		tileset.load(game) &&
		collisions.load(game);
	}
	/* Open scene document */
	GRY_JSON::Document sceneDoc;
	GRY_JSON::loadDoc(sceneDoc, scenePath);

	/* Initialize the tileset */
	tileset.setPath(sceneDoc["tilesetPath"].GetString());

	/* Initialize the tile collisions */
	collisions.setPath(sceneDoc["tilesetPath"].GetString());

	return false;
}
