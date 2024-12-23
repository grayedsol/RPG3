#include "TileSpriteAnimator.hpp"
#include "../scenes/TileMapScene.hpp"

TileSpriteAnimator::TileSpriteAnimator(TileMapScene *scene) :
	sprites(&scene->getECS().getComponent<ActorSprite>()),
	actors(&scene->getECS().getComponentReadOnly<Actor>()),
	actorAnimations(&scene->getECS().getComponent<ActorSpriteAnims>()) {
}

void TileSpriteAnimator::process(double delta) {
	for (auto e : *actorAnimations) {
		ActorSpriteAnims& animations = actorAnimations->get(e);
		if (!actors->get(e).moving) {
			sprites->get(e).index = actors->get(e).direction;
			animations.timer = 0;
			animations.index = 0;
			continue;
		}

		std::vector<Tile::TileId> animation = actors->get(e).sprinting ?
		animations.sprint[actors->get(e).direction] :
		animations.walk[actors->get(e).direction];

		sprites->get(e).index = animation[animations.index];

		animations.timer -= delta;
		if (animations.timer > 0.0) { continue; }

		if (++animations.index >= animation.size()) { animations.index = 0; }
		animations.timer += animations.duration;
		sprites->get(e).index = animation[animations.index];
	}
}