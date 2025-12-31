/**
 * @file TileMapRenderer.hpp
 * @author Grayedsol (grayedsol@gmail.com)
 * @brief @copybrief Tile::MapRenderer
 * @copyright Copyright (c) 2024
 */
#pragma once
#include "TileTileMap.hpp"
#include "TileEntityMap.hpp"

struct SDL_Renderer;

namespace Tile {
	class MapScene;
	struct MapRenderOffset;

	/**
	 * @brief Renders a tile map and tile entity map.
	 * 
	 * @details
	 * For efficiency, this renderer assumes the map uses only one tileset.
	 */
	class MapRenderer {
	private:
		/**
		 * @brief Associated MapScene class.
		 * 
		 */
		const MapScene* scene;

		/**
		 * @brief Pointer to the renderer.
		 * 
		 */
		SDL_Renderer* renderer;

		/**
		 * @brief Pointer to the Map.
		 * 
		 */
		const TileMap* tileMap;

		/**
		 * @brief Pointer to the EntityMap.
		 * 
		 */
		const EntityMap* entityMap;

		/**
		 * @brief Scaling factor for textures.
		 * 
		 */
		const float* pixelScaling;

		/**
		 * @brief Positions of entities, in game pixels.
		 * 
		 */
		const ComponentSet<Position2>* positions;

		/**
		 * @brief Sprites of entities.
		 * 
		 */
		const ComponentSet<ActorSprite>* sprites;

		/**
		 * @brief Hitboxes of entities.
		 * 
		 */
		const ComponentSet<Hitbox>* hitboxes;

		/**
		 * @copybrief MapRenderOffset
		 * 
		 */
		const MapRenderOffset* renderOffset;

		/**
		 * @brief Render a tile on the screen.
		 * 
		 * @param tileset Tileset to use.
		 * @param textureIndex Index of which tile in the Tileset to use.
		 * @param dstRect Rendering position and size information.
		 */
		void renderTile(const Tileset& tileset, const TileId textureIndex, const SDL_FRect* dstRect);

		/**
		 * @brief Render an entity's sprite on the screen.
		 * 
		 * @param e Id of the entity to render
		 */
		void renderSprite(ECS::entity e, float offsetX, float offsetY, float scaling);
	public:
		/**
		 * @brief Constructor.
		 * 
		 * @param scene Associated MapScene class.
		 */
		MapRenderer(const MapScene* scene, const TileMap* tileMap, const EntityMap* entityMap, const MapRenderOffset* renderOffset);

		/**
		 * @brief Render the TileMap and EntityMap.
		 * 
		 */
		void process();
	};
};
