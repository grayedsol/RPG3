/**
 * @file FPSScene.hpp
 * @author Grayedsol (grayedsol@gmail.com)
 * @brief @copybrief DebugScreen
 * @copyright Copyright (c) 2024
 */
#pragma once
#include "DebugText.hpp"
#include <memory>
#include <vector>
#include <type_traits>
class GRY_Game;

/**
 * @brief Displays debug information within the game.
 * 
 */
class DebugScreen {
    friend class DebugText;
private:
    /**
     * @brief Container for DebugTexts.
     * 
     */
    std::vector<std::unique_ptr<DebugText>> debugTexts;

    /**
     * @brief Font used when displaying debug text.
     * 
     */
    TTF_Font* font;

    /**
     * @brief Associated game class.
     * 
     */
    GRY_Game* game;

    /**
     * @brief Determines if the debug information will be displayed.
     * 
     */
    bool active = false;
public:
    /**
     * @brief Constructor.
     * 
     * @param game Associated game class.
     * @param color Color of the text that will be displayed.
     */
    DebugScreen(GRY_Game* game);

    /**
     * @brief Destructor.
     * 
     */
    ~DebugScreen();

    /**
     * @brief Initialize the DebugScreen.
     * 
     */
    void init();

    /**
     * @brief Update and display debug info if active.
     */
    void process();

    /**
     * @brief Switch whether the debug text is displayed or not.
     */
    void toggle() { active = !active; }

    /**
     * @brief Add debug text to the debug screen.
     * 
     * @tparam T Type of debug text to make. Must be derived from DebugText.
     * @param color Color of the debug text.
     */
    template <typename T>
    void addDebugText(SDL_Color color = SDL_Color{255, 255, 255, 255}) {
        static_assert(std::is_base_of<DebugText, T>::value);
        debugTexts.push_back(std::make_unique<T>(this, color));
    }

    /**
     * @brief Add debug text to the debug screen.
     * 
     * @details
     * The DebugScreen will take ownership of `debugText`.
     * 
     * @param debugText Pointer to an object derived from DebugText.
     */
    void addDebugText(DebugText* debugText) {
        debugTexts.push_back(std::unique_ptr<DebugText>(debugText));
    }
};