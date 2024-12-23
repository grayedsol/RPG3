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

static entity registerEntity(TileEntityMap& eMap, const GRY_JSON::Value& entityData, float normalTileSize);

static void registerPosition(TileEntityMap& eMap, entity e, const GRY_JSON::Value& pos, float normalTileSize);
static void registerActor(TileEntityMap& eMap, entity e, const GRY_JSON::Value& actor);
static void registerActorSprite(TileEntityMap& eMap, entity e, const GRY_JSON::Value& actorSprite);
static void registerActorSpriteAnimations(TileEntityMap& eMap, entity e);
static void registerPlayer(TileEntityMap& eMap, entity e);
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
			entity e = registerEntity(*this, entityData, normalTileSize);
			entityLayer.push_back(e);
		}
		sortEntityLayer(this->ecs->getComponent<Position2>(), entityLayer);
		entityLayers.push_back(entityLayer);
	}

	/* Return false normally, but if there were no layers we can return true. */
	return doc["layers"].GetArray().Size() == 0;
}

entity registerEntity(TileEntityMap& eMap, const GRY_JSON::Value& entityData, float normalTileSize) {
	entity e = eMap.ecs->createEntity();

	GRY_Assert(entityData.HasMember("position"),
		"[TileEntityMap] An entity did not have a position component.\n"
	);
	registerPosition(eMap, e, entityData["position"], normalTileSize);
	if (entityData.HasMember("actor")) { registerActor(eMap, e, entityData["actor"]); }
	if (entityData.HasMember("actorSprite")) { registerActorSprite(eMap, e, entityData["actorSprite"]); }
	if (entityData.HasMember("player")) { registerPlayer(eMap, e); }

	return e;
}

void registerPosition(TileEntityMap& eMap, entity e, const GRY_JSON::Value& pos, float normalTileSize) {
	Position2 position;

	position[0] = pos.GetArray()[0].GetFloat() * normalTileSize;
	position[1] = pos.GetArray()[1].GetFloat() * normalTileSize;

	eMap.ecs->getComponent<Position2>().add(e, position);
	eMap.ecs->getComponent<Velocity2>().add(e, Velocity2(0,0));
}

void registerActor(TileEntityMap& eMap, entity e, const GRY_JSON::Value& actor) {
	Actor data;

	data.direction = static_cast<Actor::Direction>(actor["direction"].GetUint());
	data.speed = actor["speed"].GetFloat();

	GRY_Assert(static_cast<Actor::Direction>(data.direction) > 0 &&
		static_cast<Actor::Direction>(data.direction) < 9,
		"[TileEntityMap] Actor direction must be between 1 and 8 inclusive."
	);

	eMap.ecs->getComponent<Actor>().add(e, data);
}

void registerActorSprite(TileEntityMap& eMap, entity e, const GRY_JSON::Value& actorSprite) {
	ActorSprite sprite;

	sprite.offsetX = 0;
	sprite.offsetY = 0;
	sprite.index = actorSprite["index"].GetUint();
	sprite.tileset = actorSprite["tileset"].GetUint();

	if (actorSprite.HasMember("offsetX")) { sprite.offsetX = actorSprite["offsetX"].GetFloat(); }
	if (actorSprite.HasMember("offsetY")) { sprite.offsetY = actorSprite["offsetY"].GetFloat(); }

	eMap.ecs->getComponent<ActorSprite>().add(e, sprite);

	registerActorSpriteAnimations(eMap, e);
}

void registerPlayer(TileEntityMap& eMap, entity e) {
	eMap.ecs->getComponent<Player>().add(e, Player{});
}

void registerActorSpriteAnimations(TileEntityMap &eMap, entity e) {
	ActorSpriteAnimations anims;
	Tileset& tileset = eMap.tilesets.at(eMap.ecs->getComponent<ActorSprite>().get(e).tileset);

	for (int i = 1; i < Actor::Direction::SIZE; i++) {
		//find the animation
		TileAnimation* b = nullptr;
		for (auto& a : tileset.tileAnimations) {
			if (a.tile == i) { b = &a; break; }
		}
		GRY_Assert(b, "[TileEntityMap] Actor tileset did not have an animation at %d.", i);
		for (auto frame : b->frames) {
			anims.anims[i].push_back(frame.index);
		}
	}
	
	anims.duration = 0.1;

	eMap.ecs->getComponent<ActorSpriteAnimations>().add(e, anims);
}

void sortEntityLayer(ComponentSet<Position2>& positions, std::vector<entity>& layer) {
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
