#include "SoundResource.hpp"
#include "GRY_JSON.hpp"

bool SoundResource::load(GRY_Game* game) {
	if (!sounds.empty()) { return true; }
	GRY_JSON::Document soundDoc;
	GRY_JSON::loadDoc(soundDoc, path);

	for (auto& sound : soundDoc["sounds"].GetArray()) {
		sounds.push_back(audio->loadSound(sound.GetString()));
	}

	return soundDoc["sounds"].GetArray().Size() == 0;
}