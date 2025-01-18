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

void Scene::activateControlScheme() { game->setControlScheme(controls); }

void Scene::deactivateControlScheme() { game->resetControlScheme(); }

GCmd Scene::readInput() { return game->getInput(); }

GCmd Scene::readSingleInput() const { return game->getSingleInput(); }

const bool Scene::isPressing(GCmd cmd) const { return game->isPressing(cmd); }
