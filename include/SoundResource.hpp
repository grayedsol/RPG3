#pragma once
#include "FileResource.hpp"
#include "GRY_Audio.hpp"
#include <vector>

struct SoundResource : public FileResource {
	using Sound = FMOD_SOUND;
	
	std::vector<Sound*> sounds;

	GRY_Audio* audio = nullptr;

	SoundResource(GRY_Audio* audio) : audio(audio) {}
	SoundResource(GRY_Audio* audio, const char* path) : audio(audio), FileResource(path) {}

	~SoundResource() {
		for (auto sound : sounds) { audio->releaseSound(sound); }
	}

	SoundResource(const SoundResource&) = delete;
	SoundResource& operator=(const SoundResource&) = delete;

	friend void swap(SoundResource& lhs, SoundResource& rhs) {
		using std::swap;
		swap(lhs.sounds, rhs.sounds);
	}

	SoundResource(SoundResource&& other) { swap(*this, other); }

	bool load(GRY_Game* game) final;
};