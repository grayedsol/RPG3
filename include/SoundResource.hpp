#pragma once
#include "FileResource.hpp"
#include <vector>

class FMOD_SOUND;

struct SoundResource : public FileResource {
	using Sound = FMOD_SOUND;
	
	std::vector<Sound*> sounds;

	SoundResource() = default;
	SoundResource(const char* path) : FileResource(path) {}

	~SoundResource();

	SoundResource(const SoundResource&) = delete;
	SoundResource& operator=(const SoundResource&) = delete;

	friend void swap(SoundResource& lhs, SoundResource& rhs) {
		using std::swap;
		swap(lhs.sounds, rhs.sounds);
	}

	SoundResource(SoundResource&& other) { swap(*this, other); }

	bool load(GRY_Game* game) final;
};