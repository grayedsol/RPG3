#include "SoundResource.hpp"
#include "GRY_Game.hpp"
#include "GRY_JSON.hpp"

SoundResource::~SoundResource() {
	for (auto sound : sounds) { GRY_Audio::releaseSound(sound); }
}

bool SoundResource::load(GRY_Game* game) {
	if (!sounds.empty()) { return true; }
	GRY_JSON::Document soundDoc;
	GRY_JSON::loadDoc(soundDoc, path);

	for (auto& sound : soundDoc["sounds"].GetArray()) {
		sounds.push_back(game->getAudio().loadSound(sound.GetString()));
	}

	return soundDoc["sounds"].GetArray().Size() == 0;
}