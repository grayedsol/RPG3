/**
 * @file GRY_Texture.cpp
 * @author Grayedsol (grayedsol@gmail.com)
 * @copyright Copyright (c) 2024
 */
#include "GRY_Texture.hpp"
#include "GRY_Game.hpp"

bool GRY_Texture::load(GRY_Game* game) {
    if (texture) { return true; }

    texture = game->getSDL().loadTexture(path);
    return false;
}