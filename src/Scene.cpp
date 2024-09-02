/**
 * @file Scene.cpp
 * @author Grayedsol (grayedsol@gmail.com)
 * @copyright Copyright (c) 2024
 */
#include "Scene.hpp"
#include "GRY_Game.hpp"
#include "DebugText.hpp"

Scene::~Scene() {
    delete[] scenePath;
    delete debugText;
}

GCmd Scene::readInput() { return controls.commands[game->getInput()]; }

GCmd Scene::readSingleInput() { return controls.commands[game->getSingleInput()]; }

const bool Scene::isPressing(GCmd cmd) { return game->isPressing(controls.buttons[cmd]); }
