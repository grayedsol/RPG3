/**
 * @file TileMapMovement.hpp
 * @author Grayedsol (grayedsol@gmail.com)
 * @brief @copybrief Tile::MapMovement
 * @copyright Copyright (c) 2024
 */
#pragma once
#include "TileEntityMap.hpp"
#include "GRY_PixelGame.hpp"
#include "QuadTree.hpp"

namespace Tile {
	class MapScene;

	/**
	 * @brief Moves entities in a TileMap.
	 * 
	 */
	class MapMovement {
	private:
		/**
		 * @brief Associated MapScene class.
		 * 
		 */
		MapScene* scene;

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

		const ComponentSet<MapEntity>* mapEntities;

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

		ComponentSet<MapCollisionInteraction>* collisionInteractions;

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
		 * @param attempts Number of attempts at recursively resolving the collison
		 * @return The resulting hitbox that has no conflicting collisions
		 */
		Hitbox handleEntityCollisions(Hitbox box, ECS::entity e, int layer, unsigned attempts = 0);

		/**
		 * @brief Recursively resolve collisions for an entity against tiles.
		 * 
		 * @param box The new hitbox of the entity
		 * @param layer Layer that the entity is on
		 * @return The resulting Hitbox that has no conflicting collisions
		 */
		Hitbox handleTileCollisions(Hitbox box, int layer);

		void handleSoftEntityCollisions(Hitbox box, ECS::entity e, int layer);
	public:
		/**
		 * @brief Constructor.
		 * 
		 * @param scene Associated MapScene class.
		 */
		MapMovement(MapScene* scene);

		/**
		 * @brief Process the movement for all entities in the MapScene.
		 * 
		 * @param delta Delta time for game processing, in seconds.
		 */
		void process(double delta);
	};
};