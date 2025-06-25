/**
 * @file TileEntityMap.cpp
 * @author Grayedsol (grayedsol@gmail.com) 
 * @copyright Copyright (c) 2024
 */
#include "TileEntityMap.hpp"
#include "GRY_JSON.hpp"
#include "TileComponents.hpp"
#include "TileRegisterMapCommandFuncs.hpp"

using TileId = Tile::TileId;
using TilesetId = Tile::TilesetId;
using entity = ECS::entity;

static entity registerEntity(Tile::EntityMap& eMap, const GRY_JSON::Value& entityData, float normalTileSize, uint8_t layer);

static void registerPosition(Tile::EntityMap& eMap, entity e, const GRY_JSON::Value& pos, float normalTileSize);
static void registerHitbox(Tile::EntityMap& eMap, entity e, const GRY_JSON::Value& hitbox);
static void registerActor(Tile::EntityMap& eMap, entity e, const GRY_JSON::Value& actor);
static void registerActorSprite(Tile::EntityMap& eMap, entity e, const GRY_JSON::Value& actorSprite);
static void registerActorSpriteAnimations(Tile::EntityMap& eMap, entity e, const GRY_JSON::Value& actorAnimations);
static void registerPlayer(Tile::EntityMap& eMap, entity e);
static void registerNPC(Tile::EntityMap& eMap, entity e);
static void registerMapInteraction(Tile::EntityMap& eMap, entity e, float normalTileSize, const GRY_JSON::Value& interactionData);
static void registerMapCommands(Tile::EntityMap& eMap, entity e, float normalTileSize, const GRY_JSON::Value& commandData);
static void registerCollisionInteraction(Tile::EntityMap& eMap, entity e, float normalTileSize, const GRY_JSON::Value& collisionInteractionData);
static void sortEntityLayer(ComponentSet<Position2>& positions, std::vector<entity>& layer);

bool Tile::EntityMap::load(GRY_Game *game) {
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
	for (int i = 0; i < doc["layers"].GetArray().Size(); i++) {
		auto& layerData = doc["layers"].GetArray()[i];
		EntityLayer entityLayer;
		/* Load entity data */
		for (auto& entityData : layerData.GetArray()) {
			entity e = registerEntity(*this, entityData, normalTileSize, i);
			entityLayer.push_back(e);
		}
		sortEntityLayer(this->ecs->getComponent<Position2>(), entityLayer);
		entityLayers.push_back(entityLayer);
	}
	updateLayers(this);

	/* Return false normally, but if there were no layers we can return true. */
	return doc["layers"].GetArray().Size() == 0;
}

void Tile::EntityMap::sortLayer(EntityMap *entityMap, unsigned layer) {
	sortEntityLayer(entityMap->ecs->getComponent<Position2>(), entityMap->entityLayers.at(layer));
}

void Tile::EntityMap::updateLayers(EntityMap* entityMap) {
	ComponentSet<MapEntity>& mapEntities = entityMap->ecs->getComponent<MapEntity>();
	for (int layer = 0; layer < entityMap->entityLayers.size(); layer++) {
		for (auto e : entityMap->entityLayers.at(layer)) {
			mapEntities.get(e).layer = layer;
		}
	}
}

entity registerEntity(Tile::EntityMap& eMap, const GRY_JSON::Value& entityData, float normalTileSize, uint8_t layer) {
	entity e = eMap.ecs->createEntity();

	GRY_Assert(entityData.HasMember("position"),
		"[Tile::EntityMap] An entity did not have a position component.\n"
	);
	registerPosition(eMap, e, entityData["position"], normalTileSize);
	if (entityData.HasMember("hitbox")) { registerHitbox(eMap, e, entityData["hitbox"]); }
	if (entityData.HasMember("actor")) { registerActor(eMap, e, entityData["actor"]); }
	if (entityData.HasMember("actorSprite")) { registerActorSprite(eMap, e, entityData["actorSprite"]); }
	if (entityData.HasMember("actorAnimations")) { registerActorSpriteAnimations(eMap, e, entityData["actorAnimations"]); }
	if (entityData.HasMember("player")) { registerPlayer(eMap, e); }
	if (entityData.HasMember("npc")) { registerNPC(eMap, e); }
	if (entityData.HasMember("interaction")) { registerMapInteraction(eMap, e, normalTileSize, entityData["interaction"]); }
	if (entityData.HasMember("commands")) { registerMapCommands(eMap, e, normalTileSize, entityData["commands"]); }
	if (entityData.HasMember("collisionInteraction")) { registerCollisionInteraction(eMap, e, normalTileSize, entityData["collisionInteraction"]); }

	Tile::MapEntity mapEntity{ layer };
	eMap.ecs->getComponent<Tile::MapEntity>().add(e, mapEntity);

	return e;
}

void registerPosition(Tile::EntityMap& eMap, entity e, const GRY_JSON::Value& pos, float normalTileSize) {
	Position2 position;

	position[0] = pos.GetArray()[0].GetFloat() * normalTileSize;
	position[1] = pos.GetArray()[1].GetFloat() * normalTileSize;

	eMap.ecs->getComponent<Position2>().add(e, position);
	eMap.ecs->getComponent<Velocity2>().add(e, Velocity2(0,0));
}

void registerHitbox(Tile::EntityMap &eMap, entity e, const GRY_JSON::Value& hitbox) {
	GRY_Assert(eMap.ecs->getComponent<Position2>().contains(e),
		"[Tile::EntityMap] Entity with 'hitbox' must have a position."
	);
	Hitbox box;
	Position2 pos = eMap.ecs->getComponent<Position2>().get(e);
	box.x = pos.x; box.y = pos.y;
	if (hitbox.HasMember("radius")) {
		box.h = 2 * hitbox["radius"].GetFloat();
		box.w = box.h;
	}
	else if (hitbox.HasMember("width") && hitbox.HasMember("height")) {
		box.w = hitbox["width"].GetFloat();
		box.h = hitbox["height"].GetFloat();
	}
	else {
		GRY_Assert(false,
			"[Tile::EntityMap] Entity 'hitbox' did not have either a radius or a width and height."
		);
	}
	eMap.ecs->getComponent<Hitbox>().add(e, box);
	if (hitbox.HasMember("collides") && (hitbox["collides"].GetBool() == true)) {
		eMap.ecs->getComponent<Tile::Collides>().add(e, Tile::Collides{});
	}
}

void registerActor(Tile::EntityMap& eMap, entity e, const GRY_JSON::Value& actor) {
	Tile::Actor data;

	data.direction = static_cast<Tile::Direction>(actor["direction"].GetUint());
	data.speed = actor["speed"].GetFloat();

	GRY_Assert(static_cast<Tile::Direction>(data.direction) > 0 &&
		static_cast<Tile::Direction>(data.direction) < 9,
		"[Tile::EntityMap] Actor direction must be between 1 and 8 inclusive."
	);

	eMap.ecs->getComponent<Tile::Actor>().add(e, data);
	eMap.ecs->getComponent<Tile::MapCommand>().add(e, Tile::MapCommand{ .type = Tile::MAP_CMD_NONE });
}

void registerActorSprite(Tile::EntityMap& eMap, entity e, const GRY_JSON::Value& actorSprite) {
	GRY_Assert(eMap.ecs->getComponent<Hitbox>().contains(e),
		"[Tile::EntityMap] ActorSprite must have a hitbox."
	);
	Tile::ActorSprite sprite;

	sprite.offsetX = 0;
	sprite.offsetY = 0;
	sprite.index = actorSprite["index"].GetUint();
	sprite.tileset = actorSprite["tileset"].GetUint();

	if (actorSprite.HasMember("offsetX")) { sprite.offsetX = actorSprite["offsetX"].GetFloat(); }
	if (actorSprite.HasMember("offsetY")) { sprite.offsetY = actorSprite["offsetY"].GetFloat(); }

	eMap.ecs->getComponent<Tile::ActorSprite>().add(e, sprite);
}

void registerPlayer(Tile::EntityMap& eMap, entity e) {
	eMap.ecs->getComponent<Tile::Player>().add(e, Tile::Player{});
}

void registerNPC(Tile::EntityMap &eMap, entity e) {
	eMap.ecs->getComponent<Tile::NPC>().add(e, Tile::NPC{});
}

void registerMapInteraction(Tile::EntityMap& eMap, entity e, float normalTileSize, const GRY_JSON::Value& interactionData) {
	for (int i = 0; i < std::tuple_size<Tile::MapCommandTypeList>::value; i++) {
		if (strcmp(interactionData["type"].GetString(), Tile::MapCommandNames[i]) == 0) {
			Tile::MapCommand command = registerTMC_Funcs[i](eMap, e, normalTileSize, interactionData);
			eMap.ecs->getComponent<Tile::MapInteraction>().add(e, Tile::MapInteraction{command});
			return;
		}
	}
	GRY_Assert(false, "[Tile::EntityMap] Unknown map interaction command type for entity %d", e);
}

void registerMapCommands(Tile::EntityMap &eMap, entity e, float normalTileSize, const GRY_JSON::Value &commandData) {
	Tile::MapCommandList commandList;

	for (auto& command : commandData.GetArray()) {
		bool found = false;
		for (int i = 0; i < std::tuple_size<Tile::MapCommandTypeList>::value; i++) {
			if (strcmp(command["type"].GetString(), Tile::MapCommandNames[i]) == 0) {
				commandList.commands.push_back(registerTMC_Funcs[i](eMap, e, normalTileSize, command));
				found = true;
				break;
			}
		}
		GRY_Assert(found, "[Tile::EntityMap] Unknown MapCommand \"%s\".\n", command["type"].GetString());
	}

	eMap.ecs->getComponent<Tile::MapCommandList>().add(e, commandList);
}

void registerCollisionInteraction(Tile::EntityMap& eMap, entity e, float normalTileSize, const GRY_JSON::Value& collisionInteractionData) {
	const GRY_JSON::Value& command = collisionInteractionData["command"].GetObject();
	for (int i = 0; i < std::tuple_size<Tile::MapCommandTypeList>::value; i++) {
		if (strcmp(command["type"].GetString(), Tile::MapCommandNames[i]) == 0) {
			Tile::MapCommand cmd = registerTMC_Funcs[i](eMap, e, normalTileSize, command);
			Tile::MapCollisionInteraction::Mode mode = Tile::MapCollisionInteraction::Mode::PressurePlate;
			const char* modeStr = collisionInteractionData["mode"].GetString();
			if (strcmp(modeStr, "Continuous") == 0) { mode = Tile::MapCollisionInteraction::Mode::Continuous; }
			else if (strcmp(modeStr, "Fleeting") == 0) { mode = Tile::MapCollisionInteraction::Mode::Fleeting; }
			eMap.ecs->getComponent<Tile::MapCollisionInteraction>().add(e, Tile::MapCollisionInteraction{ cmd, mode });
			return;
		}
	}
	GRY_Assert(false, "[Tile::EntityMap] Unknown map interaction command type for entity %d", e);
}

void registerActorSpriteAnimations(Tile::EntityMap& eMap, entity e, const GRY_JSON::Value& actorAnimations) {
	Tile::ActorSpriteAnims anims;
	anims.duration = actorAnimations["duration"].GetDouble() / 1000.0;

	Tile::Tileset& tileset = eMap.tilesets.at(eMap.ecs->getComponent<Tile::ActorSprite>().get(e).tileset);

	for (int i = 1; i < Tile::Direction::DirectionSize; i++) {
		TileId id = actorAnimations["walk"].GetArray()[i-1].GetUint() + 1;
		Tile::Animation* b = nullptr;

		for (auto& ani : tileset.tileAnimations) {
			if (ani.tile == id) { b = &ani; break; }
		}
		GRY_Assert(b, "[Tile::EntityMap] Actor tileset did not have an animation for tile id: %d.", id-1);
		for (auto frame : b->frames) {
			anims.walk[i].push_back(frame.index);
		}
	}

	for (int i = 1; i < Tile::Direction::DirectionSize; i++) {
		TileId id = actorAnimations["sprint"].GetArray()[i-1].GetUint() + 1;
		Tile::Animation* b = nullptr;

		for (auto& ani : tileset.tileAnimations) {
			if (ani.tile == id) { b = &ani; break; }
		}
		GRY_Assert(b, "[Tile::EntityMap] Actor tileset did not have an animation for tile id: %d.", id-1);
		for (auto frame : b->frames) {
			anims.sprint[i].push_back(frame.index);
		}
	}

	eMap.ecs->getComponent<Tile::ActorSpriteAnims>().add(e, anims);
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
