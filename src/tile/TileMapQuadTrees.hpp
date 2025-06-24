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

	class MapQuadTrees {
	private:
		std::vector<QuadTree> quadtrees;
		std::vector<QuadTree> softQuadtrees;
		MapScene* scene;
		const ComponentSet<Hitbox>* hitboxes;
		const ComponentSet<Collides>* collides;
	public:
		MapQuadTrees(MapScene* scene);

		void process();

		/**
		 * @brief Initializes the system. Must be called once before using `process`.
		 * 
		 */
		void init();

		const std::vector<QuadTree>& getQuadTrees() { return quadtrees; }

		const std::vector<QuadTree>& getSoftQuadTrees() { return softQuadtrees; }
	};
};