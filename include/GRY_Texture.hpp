/**
 * @file GRY_Texture.hpp
 * @author Grayedsol (grayedsol@gmail.com)
 * @brief @copybrief GRY_Texture
 * @copyright Copyright (c) 2024
 */
#pragma once
#include "SDL3/SDL.h"
#include "SDL3_image/SDL_image.h"
#include "FileResource.hpp"

/**
 * @brief Texture resource.
 * 
*/
struct GRY_Texture : public FileResource {
	/**
	 * @brief SDL_Texture* that is owned and managed by this resource.
	 * 
	 */
	SDL_Texture* texture = nullptr;

	/**
	 * @brief Constructor.
	 * 
	 * @copydetails FileResource::FileResource
	 */
	GRY_Texture() = default;

	/**
	 * @brief Constructor.
	 * 
	 * @param path Path to the image.
	 */
	GRY_Texture(const char* path) : FileResource(path) {}

	/**
	 * @brief Destructor.
	 * 
	 */

	~GRY_Texture() { SDL_DestroyTexture(texture); texture = nullptr; }

	GRY_Texture(const GRY_Texture&) = delete;
	GRY_Texture& operator=(const GRY_Texture&) = delete;

	/**
	 * @brief Swaps the data of two GRY_Textures.
	 * 
	 * @param lhs A GRY_Texture.
	 * @param rhs Another GRY_Texture.
	 */
	friend void swap(GRY_Texture& lhs, GRY_Texture& rhs) noexcept {
		using std::swap;
		swap(static_cast<FileResource&>(lhs), static_cast<FileResource&>(rhs));
		swap(lhs.texture, rhs.texture);
	}

	/**
	 * @brief Move constructor.
	 * 
	 * @param other GRY_Texture to take data from.
	 */
	GRY_Texture(GRY_Texture&& other) noexcept { swap(*this, other); }

	/**
	 * Load the texture from the stored image file path.
	 * 
	 * @param game Associated game class.
	 * 
	 * @returns `true` if the texture was already loaded.
	 * @returns `false` otherwise.
	 */
	bool load(GRY_Game* game) final override;
};
