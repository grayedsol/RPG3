/**
 * @file TileEntityMap.cpp
 * @author Grayedsol (grayedsol@gmail.com) 
 * @copyright Copyright (c) 2024
 */
#include "TileEntityMap.hpp"
#include "GRY_JSON.hpp"
#include "TileComponents.hpp"

static ECS::entity registerEntity(TileMapECS& ecs, const GRY_JSON::Value& entityData);

static void registerPosition(TileMapECS& ecs, ECS::entity e, const GRY_JSON::Value& pos);
static void registerActor(TileMapECS& ecs, ECS::entity e, const GRY_JSON::Value& actor);
static void registerActorTextures(TileMapECS& ecs, ECS::entity e, const GRY_JSON::Value& actorTextures);
static void registerPlayer(TileMapECS& ecs, ECS::entity e);

static void sortEntityLayer(ComponentSet<Position2>& positions, std::vector<ECS::entity>& layer);

bool TileEntityMap::load(GRY_Game *game) {
	if (!entityLayers.empty()) { return true; }
	GRY_JSON::Document doc;
	GRY_JSON::loadDoc(doc, path);

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
			entity e = registerEntity(*ecs, entityData);
			entityLayer.push_back(e);
		}
		sortEntityLayer(this->ecs->getComponent<Position2>(), entityLayer);
		entityLayers.push_back(entityLayer);
	}

	/* Return false normally, but if there were no layers we can return true. */
	return doc["layers"].GetArray().Size() == 0;
}

static ECS::entity registerEntity(TileMapECS& ecs, const GRY_JSON::Value& entityData) {
	ECS::entity e = ecs.createEntity();

	GRY_Assert(entityData.HasMember("position"),
		"[TileEntityMap] An entity did not have a position component.\n"
	);
	registerPosition(ecs, e, entityData["position"]);
	if (entityData.HasMember("actor")) { registerActor(ecs, e, entityData["actor"]); }
	if (entityData.HasMember("actorTextures")) { registerActorTextures(ecs, e, entityData["actorTextures"]); }
	if (entityData.HasMember("player")) { registerPlayer(ecs, e); }

	return e;
}

static void registerPosition(TileMapECS& ecs, ECS::entity e, const GRY_JSON::Value& pos) {
	Position2 position;
	position[0] = pos.GetArray()[0].GetFloat();
	position[1] = pos.GetArray()[1].GetFloat();
	ecs.getComponent<Position2>().add(e, position);
}

static void registerActor(TileMapECS& ecs, ECS::entity e, const GRY_JSON::Value& actor) {
	Actor::Direction direction = static_cast<Actor::Direction>(actor["direction"].GetUint());
	uint8_t tileset = actor["tileset"].GetUint();
	float speed = actor["speed"].GetFloat();
	Actor data{ speed, direction, tileset };
	ecs.getComponent<Actor>().add(e, data);
}

static void registerActorTextures(TileMapECS& ecs, ECS::entity e, const GRY_JSON::Value& actorTextures) {
	GRY_Assert(actorTextures.GetArray().Size() >= Actor::Direction::SIZE,
		"[TileEntityMap] Actor textures array must be at least Actor::Direction::SIZE. Was %d.\n",
		actorTextures.GetArray().Size()
	);
	ActorTexture textures;
	for (int i = 0; i < Actor::Direction::SIZE; i++) {
		textures.data[i] = (uint16_t)actorTextures.GetArray()[i].GetUint();
	}
	ecs.getComponent<ActorTexture>().add(e, textures);
}

static void registerPlayer(TileMapECS& ecs, ECS::entity e) {
	ecs.getComponent<Player>().add(e, Player{});
}

static void sortEntityLayer(ComponentSet<Position2>& positions, std::vector<ECS::entity>& layer) {
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
