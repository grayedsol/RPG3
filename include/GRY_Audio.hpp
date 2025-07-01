#pragma once

struct FMOD_SYSTEM;
struct FMOD_SOUND;

class GRY_Audio {
private:
	FMOD_SYSTEM* system = nullptr;
public:
	GRY_Audio();

	~GRY_Audio();

	GRY_Audio(const GRY_Audio&) = delete;
	GRY_Audio& operator=(const GRY_Audio&) = delete;

	void process();

	FMOD_SOUND* loadSound(const char* path);

	static void releaseSound(FMOD_SOUND* sound);

	void playSound(FMOD_SOUND* sound);
};