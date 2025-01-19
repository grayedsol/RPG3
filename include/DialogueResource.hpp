/**
 * @file DialogueResource.hpp
 * @author Grayedsol (grayedsol@gmail.com)
 * @brief @copybrief DialogueResource
 * @copyright Copyright (c) 2025
 */
#pragma once
#include "FileResource.hpp"
#include <vector>

struct DialogueResource : public FileResource {
	using Dialogue = std::vector<char*>;
	std::vector<Dialogue> dialogues;

	DialogueResource() = default;

	DialogueResource(const char* path) : FileResource(path) {}

	~DialogueResource() {
		for (auto& dialogue : dialogues) {
			for (auto line : dialogue) { delete[] line; }
		}
	}

	DialogueResource(const DialogueResource&) = delete;
	DialogueResource& operator=(const DialogueResource&) = delete;

	friend void swap(DialogueResource& lhs, DialogueResource& rhs) {
		using std::swap;
		swap(static_cast<FileResource&>(lhs), static_cast<FileResource&>(rhs));
		swap(lhs.dialogues, rhs.dialogues);
	}

	DialogueResource(DialogueResource&& other) noexcept { swap(*this, other); }

	bool load(GRY_Game* game) final;
};