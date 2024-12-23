/**
 * @file TileMap.cpp
 * @author Grayedsol (grayedsol@gmail.com) 
 * @copyright Copyright (c) 2024
 */
#include "TileMap.hpp"
#include "GRY_Tiled.hpp"
#include <limits>

static void insertSkips(std::vector<TileMap::TileLayer>& layers, uint32_t width) {
	Tile::entity max = std::numeric_limits<Tile::entity>::max();

	for (auto& layer : layers) {
		uint32_t numRows = (uint32_t)layer.size() / width;
		for (uint32_t y = 0; y < numRows; y++) {
			for (uint32_t x = 0; x < width; x++) {
				unsigned index = y*width+x;
				if (layer[index].id) { continue; }
				Tile& tile = layer[index];
				Tile::entity skips = 0;
				while (!layer[index+1].id && skips < max && x+1 < width) {
					skips++;
					x++;
					index++;
				}
				tile.custom = skips;
			}
		}
	}
}

bool TileMap::load(GRY_Game *game) {
	if (!tileLayers.empty()) { return true; }

	GRY_JSON::Document mapDoc;
	GRY_JSON::loadDoc(mapDoc, path);

	/* Read map width */
	width = mapDoc["width"].GetUint();

	/* Create tilesets and tile collision sets */
	if (tilesets.empty() && tileCollisions.empty() && mapDoc["tilesets"].GetArray().Size() > 0) {
		for (auto& tileset : mapDoc["tilesets"].GetArray()) {
			std::string fullPath = std::string("assets/maps/") + tileset["source"].GetString();
			tilesets.push_back(Tileset(fullPath.c_str()));
			tileCollisions.push_back(TileCollision(fullPath.c_str()));
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

	insertSkips(tileLayers, width);
	
	/* Return false normally, but if there were no layers we can return true. */
	return mapDoc["layers"].GetArray().Size() == 0;
}
