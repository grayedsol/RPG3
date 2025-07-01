#include "GRY_Audio.hpp"
#include "GRY_Log.hpp"
#include "fmod/fmod.h"
#include "fmod/fmod_errors.h"

static void GRY_FMODCheck(FMOD_RESULT result) {
	GRY_Assert(result == FMOD_OK,
		"FMOD Error! (%d) %s\n", result, FMOD_ErrorString(result)
	);
}

GRY_Audio::GRY_Audio() {
	GRY_FMODCheck(FMOD_System_Create(&system, FMOD_VERSION));

	GRY_FMODCheck(FMOD_System_Init(system, 512, FMOD_INIT_NORMAL, 0));
}

GRY_Audio::~GRY_Audio() {
	GRY_FMODCheck(FMOD_System_Release(system));
}

void GRY_Audio::process() {
	GRY_FMODCheck(FMOD_System_Update(system));
}

FMOD_SOUND* GRY_Audio::loadSound(const char* path) {
	FMOD_SOUND* sound;
	GRY_FMODCheck(FMOD_System_CreateSound(system, path, FMOD_DEFAULT, 0, &sound));
	return sound;
}

void GRY_Audio::releaseSound(FMOD_SOUND* sound) {
	GRY_FMODCheck(FMOD_Sound_Release(sound));
}

void GRY_Audio::playSound(FMOD_SOUND *sound) {
	GRY_FMODCheck(FMOD_System_PlaySound(system, sound, NULL, false, NULL));
}
