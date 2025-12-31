/**
 * @file TileMapQuadTrees.cpp
 * @author Grayedsol (grayedsol@gmail.com)
 * @copyright Copyright (c) 2025
 */
#include "TileMapQuadTrees.hpp"
#include "../scenes/TileMapScene.hpp"

Tile::MapQuadTrees::MapQuadTrees(MapScene* scene, const EntityMap* entityMap) :
	scene(scene),
	entityMap(entityMap),
	hitboxes(&scene->getECSReadOnly().getComponentReadOnly<Hitbox>()),
	collides(&scene->getECSReadOnly().getComponentReadOnly<Collides>()) {
}

void Tile::MapQuadTrees::process() {
	for (int layer = 0; layer < entityMap->entityLayers.size(); layer++) {
		quadtrees.at(layer).reset();
		softQuadtrees.at(layer).reset();
		for (auto e : entityMap->entityLayers.at(layer)) {
			if (hitboxes->contains(e)) {
				if (collides->contains(e)) {
					quadtrees.at(layer).insert(hitboxes->get(e), e);
				}
				else {
					softQuadtrees.at(layer).insert(hitboxes->get(e), e);
				}
			}
		}
	}
}

void Tile::MapQuadTrees::init(uint32_t mapWidth, uint32_t mapHeight, uint32_t numLayers) {
	Hitbox mapSize{ 0, 0,
		(float)(mapWidth * scene->getNormalTileSize()),
		(float)(mapHeight * scene->getNormalTileSize())
	};
	for (int layer = 0; layer < numLayers; layer++) {
		quadtrees.push_back(QuadTree(mapSize));
		softQuadtrees.push_back(QuadTree(mapSize));
	}
}
