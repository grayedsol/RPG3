/**
 * @file TileMapMovement.hpp
 * @author Grayedsol (grayedsol@gmail.com)
 * @copybrief TileMapMovement
 * @copyright Copyright (c) 2024
 */
#pragma once
#include "TileEntityMap.hpp"
#include "GRY_PixelGame.hpp"

class TileMapScene;
class QuadTree;

/**
 * @brief Moves entities in a TileMap.
 * 
 */
class TileMapMovement {
private:
	/**
	 * @brief Associated TileMapScene class.
	 * 
	 */
	TileMapScene* scene;

	/**
	 * @brief Positions of entities, in game pixels.
	 * 
	 */

	ComponentSet<Position2>* positions;

	/**
	 * @brief Velocities of entities, in game pixels per second.
	 * 
	 */
	ComponentSet<Velocity2>* velocities;

	/**
	 * @brief Hitboxes of entities.
	 * 
	 */
	ComponentSet<Hitbox>* hitboxes;

	/**
	 * @brief Actor data of entities.
	 * 
	 */
	ComponentSet<Actor>* actors;

	/**
	 * @brief Sprites of entities.
	 * 
	 */
	ComponentSet<ActorSprite>* sprites;

	/**
	 * @brief Set of entities that are players.
	 * 
	 */
	const ComponentSet<Player>* players;

	void glide(double delta, Velocity2 prevVelocity, ECS::entity e);

	Hitbox handleEntityCollisions(Hitbox box, QuadTree* tree, int layer);

	Hitbox handleTileCollisions(Hitbox box, int layer);
public:
	/**
	 * @brief Constructor.
	 * 
	 * @param scene Associated TileMapScene class.
	 */
	TileMapMovement(TileMapScene* scene);

	/**
	 * @brief Process the movement for all entities in the TileMapScene.
	 * 
	 * @param delta Delta time for game processing, in seconds.
	 */
	void process(double delta);
};