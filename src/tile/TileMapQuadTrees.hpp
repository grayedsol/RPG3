/**
 * @file TileMapQuadTrees.hpp
 * @author Grayedsol (grayedsol@gmail.com)
 * @brief @copybrief TileMapQuadTrees
 * @copyright Copyright (c) 2025
 */
#pragma once
#include "QuadTree.hpp"
#include "Components.hpp"
#include "ECS.hpp"

class TileMapScene;

class TileMapQuadTrees {
private:
	std::vector<QuadTree> quadtrees;
	TileMapScene* scene;
	const ComponentSet<Hitbox>* hitboxes;
public:
	TileMapQuadTrees(TileMapScene* scene);

	void process();

	/**
	 * @brief Initializes the system. Must be called once before using `process`.
	 * 
	 */
	void init();

	const std::vector<QuadTree>& getQuadTrees() { return quadtrees; }
};