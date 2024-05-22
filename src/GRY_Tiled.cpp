/**
 * @file GRY_Tiled.cpp
 * @author your name (you@domain.com)
 * @copyright Copyright (c) 2024
 */
#include "GRY_Tiled.hpp"
#include "GRY_Log.hpp"

const GRY_Tiled::Value& GRY_Tiled::getProperty(const GRY_JSON::Value& value, const char* name) {
    assert(value.HasMember("properties"));
    for (auto& elem : value["properties"].GetArray()) {
        if (strcmp(name, elem["name"].GetString()) == 0) {
            return elem["value"];
        }
    }
    GRY_Log("GRY_JSON::getProperty: Could not find property \"%s\".", name);
    return value["properties"].GetArray()[0];
}

SDL_FRect GRY_Tiled::getRect(const Value& value) {
    /* Ensure no polyline or point members */
    if (value.HasMember("polyline") || value.HasMember("point")) {
        GRY_Log("GRY_Tiled::getRect: Tiled object cannot be polyline or point.\n");
        return SDL_FRect{ 0, 0, 0, 0 };
    }

    return SDL_FRect{
        value["x"].GetFloat(),
        value["y"].GetFloat(),
        value["width"].GetFloat(),
        value["height"].GetFloat()
    };
}