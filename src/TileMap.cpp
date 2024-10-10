/**
 * @file TileMap.cpp
 * @author Grayedsol (grayedsol@gmail.com) 
 * @copyright Copyright (c) 2024
 */
#include "TileMap.hpp"
#include "GRY_Tiled.hpp"

bool TileMap::load(GRY_Game *game) {
	if (!tileLayers.empty()) { return true; }

	GRY_JSON::Document mapDoc;
	GRY_JSON::loadDoc(mapDoc, path);

	/* Read map width */
	width = mapDoc["width"].GetUint();

	/* Create tilesets and tile collision sets */
	if (tilesets.empty() && tileCollisions.empty() && mapDoc["tilesets"].GetArray().Size() > 0) {
		for (auto& tileset : mapDoc["tilesets"].GetArray()) {
			tilesets.push_back(Tileset(tileset["source"].GetString()));
			tileCollisions.push_back(TileCollision(tileset["source"].GetString()));
		}
	}

	/* Load tilesets one by one */
	for (auto& tileset : tilesets) {
		if (!tileset.load(game)) { return false; }
	}

	/* Load tile collision sets one by one */
	for (auto& tileCollision : tileCollisions) {
		if (!tileCollision.load(game)) { return false; }
	}

	/* Load tile layers */
	for (auto& layer : mapDoc["layers"].GetArray()) {
		std::vector<Tile> tiles;
		if (!layer.HasMember("data")) { continue; }
		for (auto& tile : layer["data"].GetArray()) {
			Tile::TileId tileId = tile.GetUint();
			uint8_t tilesetId = 0;
			while (tileId > tilesets.at(tilesetId).size()) {
				tileId -= tilesets.at(tilesetId).size();
				tilesetId++;
			}
			tiles.push_back(Tile{ tileId, tilesetId });
		}
		tileLayers.push_back(tiles);
	}
	
	/* Return false normally, but if there were no layers we can return true. */
	return mapDoc["layers"].GetArray().Size() == 0;
}
