/**
 * @file TileEntityMap.cpp
 * @author Grayedsol (grayedsol@gmail.com) 
 * @copyright Copyright (c) 2024
 */
#include "TileEntityMap.hpp"
#include "GRY_JSON.hpp"
#include "TileComponents.hpp"

using TileId = Tile::TileId;
using TilesetId = Tile::TilesetId;
using entity = ECS::entity;

static entity registerEntity(TileMapECS& ecs, const GRY_JSON::Value& entityData, float normalTileSize);

static void registerPosition(TileMapECS& ecs, entity e, const GRY_JSON::Value& pos, float normalTileSize);
static void registerActor(TileMapECS& ecs, entity e, const GRY_JSON::Value& actor);
static void registerActorSprite(TileMapECS& ecs, entity e, const GRY_JSON::Value& actorSprite);
static void registerPlayer(TileMapECS& ecs, entity e);
static void sortEntityLayer(ComponentSet<Position2>& positions, std::vector<entity>& layer);

bool TileEntityMap::load(GRY_Game *game) {
	if (!entityLayers.empty()) { return true; }
	GRY_JSON::Document doc;
	GRY_JSON::loadDoc(doc, path);

	/* Get the normal tile size */
	float normalTileSize = doc["normalTileSize"].GetFloat();

	/* Create tilesets */
	if (tilesets.empty() && doc["tilesets"].GetArray().Size() > 0) {
		for (auto& tileset : doc["tilesets"].GetArray()) {
			tilesets.push_back(Tileset(tileset.GetString()));
		}
	}

	/* Load tilesets one by one */
	for (auto& tileset : tilesets) {
		if (!tileset.load(game)) { return false; }
	}

	/* Load entity layer data */
	for (auto& layer : doc["layers"].GetArray()) {
		EntityLayer entityLayer;
		/* Load entity data */
		for (auto& entityData : layer.GetArray()) {
			entity e = registerEntity(*ecs, entityData, normalTileSize);
			entityLayer.push_back(e);
		}
		sortEntityLayer(this->ecs->getComponent<Position2>(), entityLayer);
		entityLayers.push_back(entityLayer);
	}

	/* Return false normally, but if there were no layers we can return true. */
	return doc["layers"].GetArray().Size() == 0;
}

static entity registerEntity(TileMapECS& ecs, const GRY_JSON::Value& entityData, float normalTileSize) {
	entity e = ecs.createEntity();

	GRY_Assert(entityData.HasMember("position"),
		"[TileEntityMap] An entity did not have a position component.\n"
	);
	registerPosition(ecs, e, entityData["position"], normalTileSize);
	if (entityData.HasMember("actor")) { registerActor(ecs, e, entityData["actor"]); }
	if (entityData.HasMember("actorSprite")) { registerActorSprite(ecs, e, entityData["actorSprite"]); }
	if (entityData.HasMember("player")) { registerPlayer(ecs, e); }

	return e;
}

static void registerPosition(TileMapECS& ecs, entity e, const GRY_JSON::Value& pos, float normalTileSize) {
	Position2 position;

	position[0] = pos.GetArray()[0].GetFloat() * normalTileSize;
	position[1] = pos.GetArray()[1].GetFloat() * normalTileSize;

	ecs.getComponent<Position2>().add(e, position);
	ecs.getComponent<Velocity2>().add(e, Velocity2(0,0));
}

static void registerActor(TileMapECS& ecs, entity e, const GRY_JSON::Value& actor) {
	Actor data;

	data.direction = static_cast<Actor::Direction>(actor["direction"].GetUint());
	data.speed = actor["speed"].GetFloat();

	GRY_Assert(static_cast<Actor::Direction>(data.direction) > 0 &&
		static_cast<Actor::Direction>(data.direction) < 9,
		"[TileEntityMap] Actor direction must be between 1 and 8 inclusive."
	);

	ecs.getComponent<Actor>().add(e, data);
}

static void registerActorSprite(TileMapECS& ecs, entity e, const GRY_JSON::Value& actorSprite) {
	ActorSprite sprite;

	sprite.offsetX = 0;
	sprite.offsetY = 0;
	sprite.index = actorSprite["index"].GetUint();
	sprite.tileset = actorSprite["tileset"].GetUint();

	if (actorSprite.HasMember("offsetX")) { sprite.offsetX = actorSprite["offsetX"].GetFloat(); }
	if (actorSprite.HasMember("offsetY")) { sprite.offsetY = actorSprite["offsetY"].GetFloat(); }

	ecs.getComponent<ActorSprite>().add(e, sprite);
}

static void registerPlayer(TileMapECS& ecs, entity e) {
	ecs.getComponent<Player>().add(e, Player{});
}

static void sortEntityLayer(ComponentSet<Position2>& positions, std::vector<entity>& layer) {
	auto lessThan = [&](Position2 p1, Position2 p2) {
		if (p1[1] == p2[1]) { return p1[0] < p2[0]; }
		else { return p1[1] < p2[1]; }
	};

	for (int i = 1; i < layer.size(); i++) {
		int j = i;
		while (j > 0 && lessThan(positions.get(layer[i]), positions.get(layer[i-1]))) {
			std::swap(layer[j], layer[j-1]);
			j--;
		}
	}
}
