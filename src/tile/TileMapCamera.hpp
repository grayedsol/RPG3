/**
 * @file TileMapCamera.hpp
 * @author Grayedsol (grayedsol@gmail.com)
 * @brief @copybrief Tile::MapCamera
 * @copyright Copyright (c) 2025
 */
#pragma once
#include "Components.hpp"
#include "TileComponents.hpp"

namespace Tile {
	class MapScene;

	/**
	 * @brief Adjusts a MapScene's rendering offset based on the player's position.
	 * 
	 */
	class MapCamera {
	private:
		/**
		 * @brief Associated MapScene class.
		 * 
		 */
		MapScene* scene;

		/**
		 * @brief Scaling factor for the rendering offset.
		 * 
		 */
		const float* pixelScaling;

		/**
		 * @brief The center coordinate of the screen used to calculate rendering offset.
		 * 
		 */
		Position2 center;

		/**
		 * @brief Hitboxes of entities.
		 * 
		 */
		const ComponentSet<Hitbox>* hitboxes;

		/**
		 * @brief Positions of entities.
		 * 
		 */
		const ComponentSet<Position2>* positions;

		/**
		 * @brief Set of entities that are players.
		 * 
		 * @details
		 * There should really only be one player.
		 */
		const ComponentSet<Player>* players;

		bool cameraLocked = true;
	public:
		/**
		 * @brief Constructor.
		 * 
		 * @param scene Associated MapScene class.
		 */
		MapCamera(MapScene* scene);

		/**
		 * @brief Sets the rendering offset based on the player's position.
		 * 
		 */
		void process();

		void lockCamera() { cameraLocked = true; }

		void unlockCamera() { cameraLocked = false; }

		bool cameraIsLocked() const { return cameraLocked; }

		bool moveCamera(Position2 position, double speed, double delta);

		bool moveCameraToPlayer(double speed, double delta);
	};
};