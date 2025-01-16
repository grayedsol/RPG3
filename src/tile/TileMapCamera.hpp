/**
 * @file TileMapCamera.hpp
 * @author Grayedsol (grayedsol@gmail.com)
 * @copybrief TileMapCamera
 * @copyright Copyright (c) 2025
 */
#pragma once
#include "Components.hpp"
#include "TileComponents.hpp"

class TileMapScene;

/**
 * @brief Adjusts a TileMapScene's rendering offset based on the player's position.
 * 
 */
class TileMapCamera {
private:
	/**
	 * @brief Associated TileMapScene class.
	 * 
	 */
	TileMapScene* scene;

	/**
	 * @brief Scaling factor for the rendering offset.
	 * 
	 */
	const float* pixelScaling;

	/**
	 * @brief Hitboxes of entities.
	 * 
	 */
	const ComponentSet<Hitbox>* hitboxes;

	/**
	 * @brief Set of entities that are players.
	 * 
	 * @details
	 * There should really only be one player.
	 */
	const ComponentSet<Player>* players;

public:
	/**
	 * @brief Constructor.
	 * 
	 * @param scene Associated TileMapScene class.
	 */
	TileMapCamera(TileMapScene* scene);

	/**
	 * @brief Sets the rendering offset based on the player's position.
	 * 
	 */
	void process();
};