/**
 * @file ExampleScene.hpp
 * @author Grayedsol (grayedsol@gmail.com)
 * @brief @copybrief ExampleScene
 * @copyright Copyright (c) 2024
 */
#pragma once
#include "Scene.hpp"
#include "GRY_Texture.hpp"

/**
 * @brief Simple game scene for use as an example.
 * 
 * @details
 * The scene displays an image from the assets folder on the screen.
 * 
 * The player can press GAME_A to switch to a new instance
 * of this scene with a FadeToBlack transition, or GAME_B to exit.
 */
class ExampleScene : public Scene {
private:
    /**
     * @brief Image that will be displayed during the scene.
     * 
     */
    GRY_Texture examplePng = GRY_Texture(nullptr);

    /**
     * @copybrief Scene::setControls
     */
    void setControls() final override;
public:
    /**
     * @brief Constructor.
     * 
     * @param game Associated game class.
     * @param path File path to the image to be displayed.
     */
    ExampleScene(GRY_Game* game, const char* imagePath) : Scene(game, imagePath) {}

    /**
     * @brief Initializes the scene.
     * 
     * @details
     * This scene is simple, so nothing is done.
     */
    void init() final override {}

    /**
     * @copydoc Scene::process
     */
    void process();

    /**
     * @copydoc Scene::load
     */
    bool load() final override;
};
