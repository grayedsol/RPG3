#include "GRY_Audio.hpp"
#include "GRY_Log.hpp"
#include "fmod/fmod_errors.h"

static void GRY_FMODCheck(FMOD_RESULT result) {
	GRY_Assert(result == FMOD_OK,
		"FMOD Error! (%d) %s\n", result, FMOD_ErrorString(result)
	);
}

GRY_Audio::GRY_Audio() {
	GRY_FMODCheck(FMOD::System_Create(&system));

	GRY_FMODCheck(system->init(512, FMOD_INIT_NORMAL, 0));
}