/**
 * @file BattleFXResource.hpp
 * @author Grayedsol (grayedsol@gmail.com)
 * @brief @copybrief Battle::FX
 * @copyright Copyright (c) 2026
 */
#pragma once
#include "BattleTypes.hpp"
#include "FileResource.hpp"
#include "SDL3/SDL_rect.h"

// effect resource
struct SDL_Texture;

namespace Battle {
	class BattleScene;

	struct FXResource : public FileResource {
		/**
		 * @brief The texture of the effect.
		 * 
		 */
		SDL_Texture* texture = nullptr;

		/**
		 * @brief Rectangles that define the space of each frame in the animation.
		 * 
		 */
		std::vector<SDL_FRect> sourceRects;

		/**
		 * @brief Width of each frame, in pixels.
		 * 
		 */
		float frameWidth = 0.0f;

		/**
		 * @brief Height of each frame, in pixels.
		 * 
		 */
		float frameHeight = 0.0f;
		
		FXResource() = default;
		FXResource(const char* path) : FileResource(path) {}

		~FXResource();

		FXResource(const FXResource&) = delete;
		FXResource& operator=(const FXResource&) = delete;

		friend void swap(FXResource& lhs, FXResource& rhs) {
			using std::swap;
			swap(static_cast<FileResource&>(lhs), static_cast<FileResource&>(rhs));
			swap(lhs.texture, rhs.texture);
			swap(lhs.sourceRects, rhs.sourceRects);
			swap(lhs.frameWidth, rhs.frameWidth);
			swap(lhs.frameHeight, rhs.frameHeight);
		}

		FXResource(FXResource&& other) noexcept { swap(*this, other); }

		/**
		 * @copydoc FileResource::load
		 * 
		 */
		bool load(GRY_Game* game) final;

		static SDL_FRect createSourceRect(int textureIndex, int tilesetWidth, int tileWidth, int tileHeight) {
			return SDL_FRect{
				(float)(textureIndex % tilesetWidth) * tileWidth,
				(float)(textureIndex / tilesetWidth) * tileHeight,
				(float)tileWidth,
				(float)tileHeight
			};
		}
	};
}