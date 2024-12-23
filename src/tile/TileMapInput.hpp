/**
 * @file TileMapInput.hpp
 * @author Grayedsol (grayedsol@gmail.com)
 * @copybrief TileMapInput
 * @copyright Copyright (c) 2024
 */
#pragma once
#include "Tile.hpp"
#include "TileEntityMap.hpp"

class TileMapScene;

class TileMapInput {
private:
	/**
	 * @brief Associated TileMapScene class.
	 * 
	 */
	TileMapScene* scene;

	/**
	 * @brief Actor data of entities.
	 * 
	 */
	ComponentSet<Actor>* actors;

	/**
	 * @brief Set of entities that are players.
	 * 
	 */
	const ComponentSet<Player>* players;
public:
	TileMapInput(TileMapScene* scene);

	void process();
};