/**
 * @file TileMapRenderer.hpp
 * @author Grayedsol (grayedsol@gmail.com)
 * @copybrief  TileMapRenderer
 * @copyright Copyright (c) 2024
 */
#pragma once
#include "TileMap.hpp"
#include "GRY_PixelGame.hpp"

class TileMapScene;

/**
 * @brief Renders a tile map.
 * 
 * @details
 * For efficiency, this renderer assumes the map uses only one tileset.
 */
class TileMapRenderer {
	using TileId = Tile::TileId;
private:
	/**
	 * @brief Associated TileMapScene class.
	 * 
	 */
	const TileMapScene* scene;

	/**
	 * @brief Reference to the renderer.
	 * 
	 */
	SDL_Renderer*& renderer;

	/**
	 * @brief Scaling factor for textures. Ideally a whole number.
	 * 
	 */
	const float& pixelScaling;

	/**
	 * @brief Render a tile on the screen.
	 * 
	 * @param tileset Tileset to use.
	 * @param textureIndex Index of which tile in the Tileset to use.
	 * @param dstRect Rendering position and size information.
	 */
	void renderTile(const Tileset& tileset, const TileId textureIndex, const SDL_FRect* dstRect);
public:
	/**
	 * @brief Constructor.
	 * 
	 * @param scene Associated TileMapScene class.
	 */
	TileMapRenderer(const TileMapScene* scene);

	/**
	 * @brief Render the TileMap.
	 * 
	 * @param map TileMap to render.
	 */
	void process(const TileMap& map);
};