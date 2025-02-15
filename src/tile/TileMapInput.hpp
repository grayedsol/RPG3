/**
 * @file TileMapInput.hpp
 * @author Grayedsol (grayedsol@gmail.com)
 * @brief @copybrief TileMapInput
 * @copyright Copyright (c) 2024
 */
#pragma once
#include "Tile.hpp"
#include "TileEntityMap.hpp"

class TileMapScene;

/**
 * @brief Processes input for a TileMapScene.
 * 
 */
class TileMapInput {
private:
	/**
	 * @brief Associated TileMapScene class.
	 * 
	 */
	TileMapScene* scene;

	/**
	 * @brief Hitboxes of entities.
	 * 
	 */
	const ComponentSet<Hitbox>* hitboxes;

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

	const ComponentSet<NPC>* npcs;

	const ComponentSet<TileMapAction>* actions;

	bool interact();
public:
	/**
	 * @brief Constructor.
	 * 
	 * @param scene Associated TileMapScene class.
	 */
	TileMapInput(TileMapScene* scene);

	/**
	 * @brief Scan and process inputs.
	 * 
	 */
	void process();
};