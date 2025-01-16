/**
 * @file TileMapMovement.hpp
 * @author Grayedsol (grayedsol@gmail.com)
 * @brief @copybrief TileMapMovement
 * @copyright Copyright (c) 2024
 */
#pragma once
#include "TileEntityMap.hpp"
#include "GRY_PixelGame.hpp"
#include "QuadTree.hpp"

class TileMapScene;

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

	/**
	 * @brief Stores quadtrees for the collision hitboxes of entities for each layer.
	 * 
	 */
	std::vector<QuadTree> trees;

	/**
	 * @brief Attempt to glide an entity so that it aligns to the nearest pixel.
	 * 
	 * @param delta Delta time for game processing, in seconds.
	 * @param prevVelocity The movement vector of the entity from the previous frame.
	 * @param e The entity to attempt the glide for.
	 */
	void glide(double delta, Velocity2 prevVelocity, ECS::entity e);

	/**
	 * @brief Recursively resolve collisions for an entity against other entities.
	 * 
	 * @param box The new hitbox of the entity
	 * @param e The id of the entity to check
	 * @param layer Layer that the entity is on
	 * @return The resulting hitbox that has no conflicting collisions
	 */
	Hitbox handleEntityCollisions(Hitbox box, ECS::entity e, int layer);

	/**
	 * @brief Recursively resolve collisions for an entity against tiles.
	 * 
	 * @param box The new hitbox of the entity
	 * @param layer Layer that the entity is on
	 * @return The resulting Hitbox that has no conflicting collisions
	 */
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

	/**
	 * @brief Initializes the system. Must be called once before using `process`.
	 * 
	 */
	void init();
};