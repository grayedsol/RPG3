#pragma once
#include "fmod/fmod.hpp"

class GRY_Audio {
private:
	FMOD::System* system = nullptr;
public:
	GRY_Audio();
};