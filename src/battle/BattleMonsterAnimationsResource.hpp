/**
 * @file BattleMonsterAnimationsResource.hpp
 * @author Grayedsol (grayedsol@gmail.com)
 * @brief @copybrief Battle::MonsterAnimationsResource
 * @copyright Copyright (c) 2026
 */
#pragma once
#include "BattleTypes.hpp"
#include "FileResource.hpp"
#include "SDL3/SDL_rect.h"

namespace Battle {
	struct MonsterAnimationsResource : public FileResource {
		using AnimationDurations = std::vector<double>;
		using AnimationIndices = std::vector<MonsterTextureId>;
		using MonsterAnimationsDurations = std::vector<AnimationDurations>;
		using MonsterAnimationsIndices = std::vector<AnimationIndices>;

		std::vector<MonsterAnimationsDurations> durations;
		std::vector<MonsterAnimationsIndices> indices;

		MonsterAnimationsResource() = default;
		MonsterAnimationsResource(const char* path) : FileResource(path) {}

		~MonsterAnimationsResource() = default;

		MonsterAnimationsResource(const MonsterAnimationsResource&) = delete;
		MonsterAnimationsResource& operator=(const MonsterAnimationsResource&) = delete;

		friend void swap(MonsterAnimationsResource& lhs, MonsterAnimationsResource& rhs) {
			using std::swap;
			swap(static_cast<FileResource&>(lhs), static_cast<FileResource&>(rhs));
			swap(lhs.durations, rhs.durations);
			swap(lhs.indices, rhs.indices);
		}

		MonsterAnimationsResource(MonsterAnimationsResource&& other) noexcept { swap(*this, other); }

		bool load(GRY_Game* game) final;
	};
}
