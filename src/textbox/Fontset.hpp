/**
 * @file Fontset.hpp
 * @author Grayedsol (grayedsol@gmail.com)
 * @brief @copybrief Fontset
 * @copyright Copyright (c) 2025
 */
#pragma once
#include "FileResource.hpp"
#include "SDL3/SDL_rect.h"
#include <vector>

struct SDL_Texture;

/**
 * @brief Parses a font texture into an array character textures.
 * 
 * @details
 * Loaded using a fontset JSON file.
 * Uses 0-based indexing.
 */
struct Fontset : public FileResource {
    using CharId = uint16_t;

    /**
     * @brief Texture the font will use.
     * 
     */
	SDL_Texture* texture = nullptr;

    /**
     * @brief Rectangles that define the space of each character in the texture.
     * 
     */
    std::vector<SDL_FRect> sourceRects;

    /**
     * @brief Standard width of each character, in pixels.
     * 
     */
    float emWidth = 0.0f;

    /**
     * @brief Height of each character, in pixels.
     * 
     */
	float charHeight = 0.0f;

	/**
	 * @brief Constructor.
	 * 
	 * @copydetails FileResource::FileResource
	 */
	Fontset() = default;

    /**
     * @brief Constructor.
     * 
     * @param path File path to the fontset JSON data.
     */
    Fontset(const char* path) : FileResource(path) {}

    /**
     * @brief Destructor.
     * 
     */
    ~Fontset();

    Fontset(const Fontset&) = delete;
    Fontset& operator=(const Fontset&) = delete;

    friend void swap(Fontset& lhs, Fontset& rhs) {
        using std::swap;
        swap(static_cast<FileResource&>(lhs), static_cast<FileResource&>(rhs));
        swap(lhs.texture, rhs.texture);
        swap(lhs.sourceRects, rhs.sourceRects);
        swap(lhs.emWidth, rhs.emWidth);
		swap(lhs.charHeight, rhs.charHeight);
    }

    Fontset(Fontset&& other) noexcept { swap(*this, other); }

    bool load(GRY_Game* game) final override;

	std::size_t size() { return sourceRects.size(); }

    /**
     * @brief Get the source rectangle for the given tile.
     * 
     * @param tileId TileId of the tile to get the source rect of.
     * @return SDL_FRect* Pointer to the tile's source rect.
     */
    const SDL_FRect* getSourceRect(CharId charId) const {
        return &sourceRects[charId];
    }
};