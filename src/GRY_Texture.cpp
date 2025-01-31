/**
 * @file GRY_Texture.cpp
 * @author Grayedsol (grayedsol@gmail.com)
 * @copyright Copyright (c) 2024
 */
#include "GRY_Texture.hpp"
#include "GRY_Game.hpp"
#include "SDL3/SDL_render.h"

GRY_Texture::~GRY_Texture() {
	SDL_DestroyTexture(texture);
	texture = nullptr;
}


bool GRY_Texture::load(GRY_Game* game) {
    if (texture) { return true; }

    texture = game->getSDL().loadTexture(path);
    return false;
}