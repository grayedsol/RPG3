/**
 * @file TileEntityMap.cpp
 * @author Grayedsol (grayedsol@gmail.com) 
 * @copyright Copyright (c) 2024
 */
#include "TileEntityMap.hpp"
#include "GRY_JSON.hpp"
#include "TileComponents.hpp"

static ECS::entity registerEntity(TileMapECS& ecs, const GRY_JSON::Value& entityData);

static void registerActor(TileMapECS& ecs, ECS::entity e, const GRY_JSON::Value& actor);

bool TileEntityMap::load(GRY_Game *game) {
	if (!entityLayers.empty()) { return true; }
	GRY_JSON::Document doc;
	GRY_JSON::loadDoc(doc, path);

	for (auto& layer : doc["layers"].GetArray()) {
		EntityLayer entityLayer;
		for (auto& entityData : layer.GetArray()) {
			entity e = registerEntity(*ecs, entityData);
			entityLayer.push_back(e);
		}
		entityLayers.push_back(entityLayer);
	}

	return false;
}

static ECS::entity registerEntity(TileMapECS& ecs, const GRY_JSON::Value& entityData) {
	ECS::entity e = ecs.createEntity();

	if (entityData.HasMember("actor")) { registerActor(ecs, e, entityData["actor"]); }

	return e;
}

static void registerActor(TileMapECS& ecs, ECS::entity e, const GRY_JSON::Value& actor) {
	Actor::Direction direction = static_cast<Actor::Direction>(actor["direction"].GetUint());
	uint8_t tileset = actor["tileset"].GetUint();
	Actor data{ direction, tileset };
	ecs.getComponent<Actor>().add(e, data);
}
