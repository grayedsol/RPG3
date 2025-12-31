/**
 * @file TileMapQuadTrees.hpp
 * @author Grayedsol (grayedsol@gmail.com)
 * @brief @copybrief Tile::MapQuadTrees
 * @copyright Copyright (c) 2025
 */
#pragma once
#include "QuadTree.hpp"
#include "Components.hpp"
#include "TileComponents.hpp"
#include "ECS.hpp"

namespace Tile {
	class MapScene;
	class EntityMap;

	class MapQuadTrees {
	private:
		std::vector<QuadTree> quadtrees;
		std::vector<QuadTree> softQuadtrees;
		MapScene* scene;
		const EntityMap* entityMap;
		const ComponentSet<Hitbox>* hitboxes;
		const ComponentSet<Collides>* collides;
	public:
		MapQuadTrees(MapScene* scene, const EntityMap* entityMap);

		void process();

		/**
		 * @brief Initializes the system. Must be called once before using `process`.
		 * 
		 * @param mapWidth Width of the map in tiles.
		 * @param mapHeight Height of the map in tiles.
		 */
		void init(uint32_t mapWidth, uint32_t mapHeight, uint32_t numLayers);

		const std::vector<QuadTree>& getQuadTrees() const { return quadtrees; }

		const std::vector<QuadTree>& getSoftQuadTrees() const { return softQuadtrees; }

		void updateQuadTree(Hitbox oldBox, Hitbox newBox, ECS::entity e, unsigned layer) {
			quadtrees.at(layer).update(oldBox, newBox, e);
		}
	};
};