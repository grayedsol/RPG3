#include "TileSpriteAnimator.hpp"
#include "../scenes/TileMapScene.hpp"

TileSpriteAnimator::TileSpriteAnimator(TileMapScene *scene) :
	sprites(&scene->getECS().getComponent<ActorSprite>()),
	actors(&scene->getECS().getComponentReadOnly<Actor>()),
	animations(&scene->getECS().getComponent<ActorSpriteAnimations>()) {
}

void TileSpriteAnimator::process(double delta) {
	for (auto e : *actors) {
		if (!actors->get(e).moving) {
			sprites->get(e).index = actors->get(e).direction;
			animations->get(e).timer = 0;
			animations->get(e).index = 0;
			continue;
		}

		std::vector<Tile::TileId> anim = animations->get(e).anims[actors->get(e).direction];
		sprites->get(e).index = anim[animations->get(e).index];

		animations->get(e).timer -= delta;
		if (animations->get(e).timer > 0.0) { continue; }

		if (++animations->get(e).index >= anim.size()) { animations->get(e).index = 0; }
		animations->get(e).timer += animations->get(e).duration;
		sprites->get(e).index = anim[animations->get(e).index];
	}
}