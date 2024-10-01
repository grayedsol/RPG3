/**
 * @file TileEntityMap.cpp
 * @author Grayedsol (grayedsol@gmail.com) 
 * @copyright Copyright (c) 2024
 */
#include "TileEntityMap.hpp"
#include "GRY_JSON.hpp"
#include "TileComponents.hpp"

bool TileEntityMap::load(GRY_Game *game) {
	return false;
}

bool TileEntityMap::load(ComponentSet<Actor>& actors) {
	GRY_JSON::Document doc;
	GRY_JSON::loadDoc(doc, path);

	for (auto& layer : doc["layers"].GetArray()) {
		for (auto& entity : layer.GetArray()) {
			if (!entity.HasMember("actor")) { continue; }
			Actor::Direction direction = static_cast<Actor::Direction>(entity["actor"]["direction"].GetUint());
			uint8_t tileset = entity["actor"]["tileset"].GetUint();
		}
	}
	return false;
}

bool TileEntityMap::load(ComponentSet<ActorTexture>& actorTextures) {
	return false;
}
