/**
 * @file TileMap.cpp
 * @author Grayedsol (grayedsol@gmail.com) 
 * @copyright Copyright (c) 2024
 */
#include "TileMap.hpp"
#include "GRY_Tiled.hpp"
#include <limits>

bool TileMap::load(GRY_Game *game) {
	if (!tileLayers.empty()) { return true; }

	GRY_JSON::Document mapDoc;
	GRY_JSON::loadDoc(mapDoc, path);

	/* Read map width */
	width = mapDoc["width"].GetUint();

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
			Tile::TileId tileId = tile.GetUint();
			tiles.push_back(Tile{ tileId });
		}
		tileLayers.push_back(tiles);
	}

	/* Load object layers */
	for (auto& layer : mapDoc["layers"].GetArray()) {
		if (strcmp(layer["type"].GetString(), "objectgroup")) { continue; }

		std::vector<SDL_FRect> rectangles;
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
		collisionRects.push_back(rectangles);
	}
	
	/* Return false normally, but if there were no layers we can return true. */
	return mapDoc["layers"].GetArray().Size() == 0;
}
