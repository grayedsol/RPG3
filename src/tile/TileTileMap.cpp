/**
 * @file TileTileMap.cpp
 * @author Grayedsol (grayedsol@gmail.com) 
 * @copyright Copyright (c) 2024
 */
#include "TileTileMap.hpp"
#include "GRY_Tiled.hpp"
#include "GRY_Game.hpp"
#include <limits>

bool Tile::TileMap::load(GRY_Game *game) {
	if (!tileLayers.empty()) { return true; }

	GRY_JSON::Document mapDoc;
	GRY_JSON::loadDoc(mapDoc, path);

	/* Read map width */
	width = mapDoc["width"].GetUint();
	/* Set height to zero for now */
	height = 0;

	/* Create tilesets and tile collision sets */
	if (!tileset.path && !tileCollision.path) {
		GRY_Assert(mapDoc["tilesets"].GetArray().Size() == 1,
			"[TileMap] Tileset must use exactly one tileset."
		);
		auto& tilesetData = mapDoc["tilesets"].GetArray()[0];
		std::string fullPath = std::string("assets/maps/") + tilesetData["source"].GetString();
		tileset.setPath(fullPath.c_str());
		tileCollision.setPath(fullPath.c_str());
	}

	/* Load the tileset */
	if (!tileset.load(game)) { return false; }

	/* Load the tile collision data */
	if (!tileCollision.load(game)) { return false; }


	/* Load tile layers */
	for (auto& layer : mapDoc["layers"].GetArray()) {
		if (strcmp(layer["type"].GetString(), "tilelayer")) { continue; }

		std::vector<Tile> tiles;
		for (auto& tile : layer["data"].GetArray()) {
			TileId tileId = tile.GetUint();
			tiles.push_back(Tile{ tileId });
		}
		height = std::max(height, (uint32_t)(tiles.size() / width));
		tileLayers.push_back(tiles);
	}

	/* Load object layers */
	for (auto& layer : mapDoc["layers"].GetArray()) {
		if (strcmp(layer["type"].GetString(), "objectgroup")) { continue; }

		std::vector<SDL_FRect> rectangles;
		rectangles.push_back(SDL_FRect{ 0, 0, 0, 0 }); /**< 0 element rect is no collision */
		for (auto& object : layer["objects"].GetArray()) {
			if (object.HasMember("polyline")) {}
			else if (!strcmp(object["type"].GetString(), "")) {
				SDL_FRect rect{ 0, 0, 0, 0 };
				rect.x = object["x"].GetFloat();
				rect.y = object["y"].GetFloat();
				rect.h = object["height"].GetFloat();
				rect.w = object["width"].GetFloat();
				rectangles.push_back(rect);
			}
		}
		GRY_Assert(collisionRects.size() < (std::size_t)std::numeric_limits<CollisionId>::max,
			"[TileMap] A layer had too many collision rectangles."
		);
		collisionRects.push_back(rectangles);
	}
	
	GRY_Assert(tileLayers.size() >= collisionRects.size(),
		"[TileMap] Cannot have more object layers than tile layers."
	);
	
	/* Return false normally, but if there were no layers we can return true. */
	return mapDoc["layers"].GetArray().Size() == 0;
}
