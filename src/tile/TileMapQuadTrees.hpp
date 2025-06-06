/**
 * @file TileMapQuadTrees.hpp
 * @author Grayedsol (grayedsol@gmail.com)
 * @brief @copybrief Tile::MapQuadTrees
 * @copyright Copyright (c) 2025
 */
#pragma once
#include "QuadTree.hpp"
#include "Components.hpp"
#include "ECS.hpp"

namespace Tile {
	class MapScene;

	class MapQuadTrees {
	private:
		std::vector<QuadTree> quadtrees;
		MapScene* scene;
		const ComponentSet<Hitbox>* hitboxes;
	public:
		MapQuadTrees(MapScene* scene);

		void process();

		/**
		 * @brief Initializes the system. Must be called once before using `process`.
		 * 
		 */
		void init();

		const std::vector<QuadTree>& getQuadTrees() { return quadtrees; }
	};
};