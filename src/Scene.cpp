/**
 * @file Scene.cpp
 * @author Grayedsol (grayedsol@gmail.com)
 * @copyright Copyright (c) 2024
 */
#include "Scene.hpp"
#include "GRY_Game.hpp"

Scene::~Scene() {
    delete[] scenePath;
}

GCmd Scene::readInput() { return controls.commands[game->getInput()]; }

GCmd Scene::readSingleInput() const { return controls.commands[game->getSingleInput()]; }

const bool Scene::isPressing(GCmd cmd) const { return game->isPressing(controls.buttons[cmd]); }
