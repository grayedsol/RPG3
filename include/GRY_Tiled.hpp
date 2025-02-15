/**
 * @file GRY_Tiled.hpp
 * @author your name (you@domain.com)
 * @brief @copybrief GRY_Tiled
 * @copyright Copyright (c) 2024
 */
#pragma once
#include "GRY_JSON.hpp"
#include "SDL3/SDL_rect.h"

/**
 * @brief Defines functions for extracting data from Tiled files in JSON format.
 * 
 */
namespace GRY_Tiled {
    /**
     * @brief @copybrief rapidjson::Document
     * 
     */
    using Document = rapidjson::Document;
    /**
     * @brief @copybrief rapidjson::Value
     * 
     */
    using Value = rapidjson::Value;

    /**
     * @brief Get a property from a Tiled object.
     * 
     * @param value Object to get property from.
     * @param name Name of the property.
     * @return Reference to the value of the property.
     */
    const Value& getProperty(const Value& value, const char* name);

    /**
     * @brief Find out if a Tiled object has a property.
     * 
     * @param value Object to search for property from.
     * @param name Name of the property.
     * @return `true` if the object has the property, `false` otherwise.
     */
	bool searchProperty(const Value& value, const char* name);

    /**
     * @brief Get a rectangle Tiled object as an SDL_FRect.
     * 
     * @param value Object to get rectangle data from.
     * @return The object as an SDL_FRect.
     */
    SDL_FRect getRect(const Value& value);
}