#include "Tile.hpp"
#include "GRY_Texture.hpp"

struct Tileset : public FileResource {
    using Id = Tile::Id;
    GRY_Texture* gtexture = nullptr;

    std::vector<SDL_FRect> sourceRects;

    std::vector<Id> textureIdx;
    std::vector<AnimatedTile> animatedTiles;

    float tileWidth = 0.0f;
	float tileHeight = 0.0f;

    Tileset(const char* path) : FileResource(path) {}
    ~Tileset() { delete gtexture; }
    Tileset(const Tileset&) = delete;
    Tileset& operator=(const Tileset&) = delete;

    friend void swap(Tileset& lhs, Tileset& rhs) {
        using std::swap;
        swap(static_cast<FileResource&>(lhs), static_cast<FileResource&>(rhs));
        swap(lhs.gtexture, rhs.gtexture);
        swap(lhs.sourceRects, rhs.sourceRects);
        swap(lhs.textureIdx, rhs.textureIdx);
        swap(lhs.animatedTiles, rhs.animatedTiles);
        swap(lhs.tileWidth, rhs.tileWidth);
		swap(lhs.tileHeight, rhs.tileHeight);
    }

    Tileset(Tileset&& other) noexcept { swap(*this, other); }

    bool load(GRY_Game* game) final override;

    static SDL_FRect createSourceRect(int textureIndex, int tilesetWidth, int tileWidth, int tileHeight) {
        return SDL_FRect{
            (float)(textureIndex % tilesetWidth) * tileWidth,
            (float)(textureIndex / tilesetWidth) * tileHeight,
            (float)tileWidth,
            (float)tileHeight
        };
    }
};