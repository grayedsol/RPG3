/**
 * @file DebugText.hpp
 * @author Grayedsol (grayedsol@gmail.com)
 * @brief @copybrief DebugText
 * @copyright Copyright (c) 2024
 */
#pragma once
#include "SDL3/SDL.h"

struct TTF_Font;
class DebugScreen;
class GRY_Game;

/**
 * @brief Stores and updates a texture displaying debug information.
 * 
 * @details
 * Meant to be inherited from. This default DebugText only displays "No debug info".
 */
struct DebugText {
protected:
    /**
     * @brief Color of the text within the texture.
     * 
     */
    const SDL_Color color;

    /**
     * @brief Texture of debug information.
     * 
     */
    SDL_Texture* texture = nullptr;

    /**
     * @brief Font of the text within the texture.
     * 
     */
    TTF_Font*& font;

    /**
     * @brief Associated game class.
     * 
     */
    GRY_Game*& game;

    /**
     * @brief Width of the texture.
     * 
     */
    float width;

    /**
     * @brief Height of the texture.
     * 
     */
    float height;

    /**
     * @brief Update the texture to display `text`.
     * 
     * @param text Text that will be displayed.
     */
    void updateText(const char* text);
public:
    /**
     * @brief Constructor.
     * 
     * @param debugScreen Pointer to the DebugScreen.
     * @param color Color of the text that will be displayed. Defaults to white.
     */
    DebugText(DebugScreen* debugScreen, SDL_Color color = SDL_Color{255,255,255,255});

    /**
     * @brief Destructor.
     * 
     */
    virtual ~DebugText();

    DebugText(const DebugText&) = delete;
    DebugText& operator=(const DebugText&) = delete;

    /**
     * @brief Render the text texture at the given coordinates.
     * 
     * @param x x coordinate of the render location.
     * @param y y coordinate of the render location.
     */
    void render(float x, float y) const;

    /**
     * @brief Get the height of the texture.
     * 
     * @return Height of the texture.
     */
    float getHeight() { return height; }

    /**
     * @brief Checks debug data and updates the text if needed.
     * 
     * @details
     * Implementations of this function should call `updateText`,
     * but only when the debug data has changed, or at timed intervals.
     */
    virtual void process();
};
