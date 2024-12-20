/**
 * @file TileMapRenderer.hpp
 * @author Grayedsol (grayedsol@gmail.com)
 * @copybrief TileMapRenderer
 * @copyright Copyright (c) 2024
 */
#pragma once
#include "TileMap.hpp"
#include "TileEntityMap.hpp"
#include "GRY_PixelGame.hpp"

class TileMapScene;

/**
 * @brief Renders a tile map and tile entity map.
 * 
 * @details
 * For efficiency, this renderer assumes the map uses only one tileset.
 */
class TileMapRenderer {
	using TileId = Tile::TileId;
	using TileLayer = TileMap::TileLayer;
	using EntityLayer = TileEntityMap::EntityLayer;
private:
	/**
	 * @brief Associated TileMapScene class.
	 * 
	 */
	const TileMapScene* scene;

	/**
	 * @brief Pointer to the renderer.
	 * 
	 */
	SDL_Renderer* renderer;

	/**
	 * @brief Pointer to the TileMap.
	 * 
	 */
	const TileMap* tileMap;

	/**
	 * @brief Pointer to the TileEntityMap.
	 * 
	 */
	const TileEntityMap* entityMap;

	/**
	 * @brief Scaling factor for textures. Ideally a whole number.
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
	void renderSprite(ECS::entity e);
public:
	/**
	 * @brief Constructor.
	 * 
	 * @param scene Associated TileMapScene class.
	 */
	TileMapRenderer(const TileMapScene* scene);

	/**
	 * @brief Render the TileMap and TileEntityMap.
	 * 
	 */
	void process();
};