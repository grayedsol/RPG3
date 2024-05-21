#include "SDL3/SDL.h"
#include "ECS.hpp"
#include <vector>

struct Tile {
    using Id = uint16_t;
    using entity = ECS::entity;

    Id id;
    uint8_t tileset;
    entity custom;
};

struct AnimatedTile {
	using Id = Tile::Id;

	struct Frame {
        /* In milliseconds */
		const double duration;
		const Id index;
	};

    const std::vector<Frame> frames;
    double timer;
    Id index;
};
