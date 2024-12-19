/**
 * @file Tileset.hpp
 * @author Grayedsol (grayedsol@gmail.com)
 * @brief @copybrief Tileset
 * @copyright Copyright (c) 2024
 */
#pragma once
#include "Tile.hpp"
#include "GRY_Texture.hpp"

/**
 * @brief Parses a texture into an array of simple tiles.
 * 
 * @details
 * Loaded using a Tiled tileset JSON file.
 * Uses 1-based indexing, due to the presence of the empty tile,
 * which uses the 0 index.
 */
struct Tileset : public FileResource {
    using TileId = Tile::TileId;

    /**
     * @brief Texture the tileset will use.
     * 
     */
	SDL_Texture* texture = nullptr;

    /**
     * @brief Rectangles that define the space of each tile in the texture.
     * 
     */
    std::vector<SDL_FRect> sourceRects;

    /**
     * @brief Indicies that refer to a source rect.
     * 
     * @details
     * Elements can be modified to refer to a different source rect,
     * thus enabling the final texture of tiles to change. Useful for
     * animated tiles.
     */
    std::vector<TileId> textureIdx;

    /**
     * @brief Container of animations for tiles.
     * 
     * @details
     * The data is used to modify textureIdx.
     */
    std::vector<TileAnimation> tileAnimations;

    /**
     * @brief Width of each tile, in pixels.
     * 
     */
    float tileWidth = 0.0f;

    /**
     * @brief Height of each tile, in pixels.
     * 
     */
	float tileHeight = 0.0f;

	/**
	 * @brief Constructor.
	 * 
	 * @copydetails FileResource::FileResource
	 */
	Tileset() = default;

    /**
     * @brief Constructor.
     * 
     * @param path File path to the Tiled tileset JSON data.
     */
    Tileset(const char* path) : FileResource(path) {}

    /**
     * @brief Destructor.
     * 
     */
    ~Tileset() {
		SDL_DestroyTexture(texture);
		texture = nullptr;
	}

    Tileset(const Tileset&) = delete;
    Tileset& operator=(const Tileset&) = delete;

    friend void swap(Tileset& lhs, Tileset& rhs) {
        using std::swap;
        swap(static_cast<FileResource&>(lhs), static_cast<FileResource&>(rhs));
        swap(lhs.texture, rhs.texture);
        swap(lhs.sourceRects, rhs.sourceRects);
        swap(lhs.textureIdx, rhs.textureIdx);
        swap(lhs.tileAnimations, rhs.tileAnimations);
        swap(lhs.tileWidth, rhs.tileWidth);
		swap(lhs.tileHeight, rhs.tileHeight);
    }

    Tileset(Tileset&& other) noexcept { swap(*this, other); }

    bool load(GRY_Game* game) final override;

    void processAnimations(double delta);

	std::size_t size() { return sourceRects.size() - 1; }

    /**
     * @brief Get the source rectangle for the given tile.
     * 
     * @param tileId TileId of the tile to get the source rect of.
     * @return SDL_FRect* Pointer to the tile's source rect.
     */
    const SDL_FRect* getSourceRect(TileId tileId) const {
        return &sourceRects[textureIdx[tileId]];
    }

    static SDL_FRect createSourceRect(int textureIndex, int tilesetWidth, int tileWidth, int tileHeight) {
        return SDL_FRect{
            (float)(textureIndex % tilesetWidth) * tileWidth,
            (float)(textureIndex / tilesetWidth) * tileHeight,
            (float)tileWidth,
            (float)tileHeight
        };
    }
};