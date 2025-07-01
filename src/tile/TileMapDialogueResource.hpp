/**
 * @file TileMapDialogueResource.hpp
 * @author Grayedsol (grayedsol@gmail.com)
 * @brief @copybrief MapDialogueResource
 * @copyright Copyright (c) 2025
 */
#pragma once
#include "FileResource.hpp"
#include "TileMapCommand.hpp"
#include <vector>

namespace Tile {
	struct MapDialogue {
		std::vector<const char*> lines;
		unsigned path1 = 0;
		unsigned path2 = 0;
		bool branching = false;
		MapCommand command = { .data { .type = MAP_CMD_NONE } };
	};

	struct MapDialogueResource : public FileResource {
		std::vector<MapDialogue> dialogues;

		MapDialogueResource() = default;

		MapDialogueResource(const char* path) : FileResource(path) {}

		~MapDialogueResource() {
			for (auto& dialogue : dialogues) {
				for (auto line : dialogue.lines) { delete[] line; }
			}
		}

		MapDialogueResource(const MapDialogueResource&) = delete;
		MapDialogueResource& operator=(const MapDialogueResource&) = delete;

		friend void swap(MapDialogueResource& lhs, MapDialogueResource& rhs) {
			using std::swap;
			swap(static_cast<FileResource&>(lhs), static_cast<FileResource&>(rhs));
			swap(lhs.dialogues, rhs.dialogues);
		}

		MapDialogueResource(MapDialogueResource&& other) noexcept { swap(*this, other); }

		bool load(GRY_Game* game) final;
	};
};
