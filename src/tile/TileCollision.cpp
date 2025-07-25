/**
 * @file TileCollision.cpp
 * @author Grayedsol (grayedsol@gmail.com)
 * @copyright Copyright (c) 2024
 */
#include "TileCollision.hpp"
#include "GRY_Game.hpp"
#include "GRY_Tiled.hpp"

bool Tile::Collision::load(GRY_Game* game) {
    if (collisions.size() > 0) { return true; }
	bool noCollisions = true;

    /* Open the tileset file */
    GRY_JSON::Document tilesetDoc;
    GRY_JSON::loadDoc(tilesetDoc, path);

    /* Load in collision rectangles */
    for (auto& tile : tilesetDoc["tiles"].GetArray()) {
        TileId id = tile["id"].GetUint() + 1; /* Add 1 because it's 1-based indexing */

        if (!tile.HasMember("objectgroup")) { continue; }
		noCollisions = false;

        const GRY_JSON::Value& objects = tile["objectgroup"]["objects"];

        /* Ensure there is a rectangle */
        GRY_Assert(objects.GetArray().Size() > 0,
			"[Tile::Collision] There were no rectangle objects in the 'objects' array for TileId: %d.\n", id
		);
        SDL_FRect collisionRect = GRY_Tiled::getRect(objects.GetArray()[0]);

        /* Add data to sparse set */
        collisions.add(id, collisionRect);
    }

	/* Return false normally, but if there were no collisions at all, we can return true. */
    return noCollisions;
}

const SDL_FRect* Tile::Collision::getCollision(TileId tile) {
    if (!collisions.contains(tile)) { return nullptr; }
    return &collisions.get(tile);
}