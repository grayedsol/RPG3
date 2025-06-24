/**
 * @file TileMapQuadTrees.cpp
 * @author Grayedsol (grayedsol@gmail.com)
 * @copyright Copyright (c) 2025
 */
#include "TileMapQuadTrees.hpp"
#include "../scenes/TileMapScene.hpp"

Tile::MapQuadTrees::MapQuadTrees(MapScene *scene) :
	scene(scene),
	hitboxes(&scene->getECSReadOnly().getComponentReadOnly<Hitbox>()),
	collides(&scene->getECSReadOnly().getComponentReadOnly<Collides>()) {
}

void Tile::MapQuadTrees::process() {
	for (int layer = 0; layer < scene->getTileEntityMap().entityLayers.size(); layer++) {
		quadtrees.at(layer).reset();
		softQuadtrees.at(layer).reset();
		for (auto e : scene->getTileEntityMap().entityLayers.at(layer)) {
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

void Tile::MapQuadTrees::init() {
	Hitbox mapSize{ 0, 0,
		(float)(scene->getTileMap().width * scene->getNormalTileSize()),
		(float)(scene->getTileMap().height * scene->getNormalTileSize())
	};
	for (int layer = 0; layer < scene->getTileEntityMap().entityLayers.size(); layer++) {
		quadtrees.push_back(QuadTree(mapSize));
		softQuadtrees.push_back(QuadTree(mapSize));
	}
}
