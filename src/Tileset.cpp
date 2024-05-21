#include "Tileset.hpp"
#include "GRY_Game.hpp"
#include "GRY_JSON.hpp"

bool Tileset::load(GRY_Game* game) {
    if (tileWidth != 0.0f) { return true; }

    GRY_JSON::Document file;
    GRY_JSON::loadDoc(file, path);

    /* Load texture */
    if (!gtexture) {
        gtexture = new GRY_Texture(file["image"].GetString());
        return gtexture->load(game);
    }

    /* Read width and height of a single tile */
	tileWidth = file["tilewidth"].GetFloat();
	tileHeight = file["tileheight"].GetFloat();
	assert(tileWidth && tileHeight);

    int texture_width;
	SDL_QueryTexture(gtexture->texture, NULL, NULL, &texture_width, NULL);
	/* Width of tileset in number of tiles */
	int tilesetWidth = texture_width / (int)tileWidth;

    /* Get total number of tiles in the tileset */
	Id tileCount = file["tilecount"].GetUint();

	/* Create texture idx */
	for (Id i = 0; i < tileCount; i++) {
		textureIdx.push_back(i);
	}

    /* Push empty SDL_FRect for 0 index */
	sourceRects.push_back(SDL_FRect{0,0,0,0});
	/* Load in source rectangles */
	for (int i = 0; i < tileCount; i++) {
		sourceRects.push_back(
			createSourceRect(i, tilesetWidth, (int)tileWidth, (int)tileHeight)
		);
	}

    for (auto& tile : file["tiles"].GetArray()) {
        Id id = tile["id"].GetUint();
        /* Load animations */
        if (tile.HasMember("animation")) {
			std::vector<AnimatedTile::Frame> frames;
			for (auto& frame : tile["animation"].GetArray()) {
				frames.push_back(AnimatedTile::Frame {
						frame["duration"].GetFloat(),
						(Id)frame["tileid"].GetUint()
					}
				);
			}
			animatedTiles.push_back(AnimatedTile{frames, frames.at(0).duration, id});
		}
    }

    return false;
}