/**
 * @file TileMapInput.hpp
 * @author Grayedsol (grayedsol@gmail.com)
 * @brief @copybrief Tile::MapInput
 * @copyright Copyright (c) 2024
 */
#pragma once
#include "Tile.hpp"
#include "TileEntityMap.hpp"

namespace Tile {
	class MapScene;

	/**
	 * @brief Processes input for a Tile::MapScene.
	 * 
	 */
	class MapInput {
	private:
		/**
		 * @brief Associated Tile::MapScene class.
		 * 
		 */
		MapScene* scene;

		/**
		 * @brief Hitboxes of entities.
		 * 
		 */
		const ComponentSet<Hitbox>* hitboxes;

		const ComponentSet<MapEntity>* mapEntities;

		/**
		 * @brief Actor data of entities.
		 * 
		 */
		ComponentSet<Actor>* actors;

		/**
		 * @brief Set of entities that are players.
		 * 
		 */
		ComponentSet<Player>* players;

		const ComponentSet<MapInteraction>* interactions;

		bool interact();
	public:
		/**
		 * @brief Constructor.
		 * 
		 * @param scene Associated Tile::MapScene class.
		 */
		MapInput(MapScene* scene);

		/**
		 * @brief Scan and process inputs.
		 * 
		 */
		void process();
	};
};