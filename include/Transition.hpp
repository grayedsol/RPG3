/**
 * @file Transition.hpp
 * @author Grayedsol (grayedsol@gmail.com)
 * @brief @copybrief Transition
 * @copyright Copyright (c) 2024
 */
#pragma once

class GRY_Game;

/**
 * @brief Abstract transition class to switch between game scenes.
 * 
 * @details
 * Aside from construction/destruction, a transition has two distinct phases:
 * * Initial - transition out of the current scene
 * * Release - transition into the new scene
 * 
 * There is an implicit "hold" phase between the initial and release
 * phases, in which the SceneManager loads the new scene.
 * 
 * @sa SceneManager
 */
class Transition {
protected:

    /**
     * @brief Associated game class
     * 
     */
    GRY_Game* game;

    /**
     * @brief Identifies the current phase.
     * 
     */
    bool releasing = false;
public:
    /**
     * @brief Constructor.
     * 
     * @param game @copybrief game
     */
    Transition(GRY_Game* game) : game(game) {}
    virtual ~Transition() = default;
    Transition(const Transition&) = delete;
    Transition& operator=(const Transition&) = delete;
    
    /**
     * @brief Update the transition.
     * 
     * @return `true` if completely finished and ready to be deleted.
     * @return `false` otherwise.
     */
    virtual bool process() = 0;

    /**
     * @brief Tell the transition to go into the release phase.
     * 
     * @details
     * Used when the new scene has finished loading.
     */
    void release() { releasing = true; }


    /**
     * @brief Determine if the initial phase is complete.
     * 
     * @return `true` if the transition has finished the initial phase.
     * @return `false` otherwise. 
     */
    virtual bool isReadyToRelease() = 0;
};
