#include "BattlePaletteRenderer.hpp"
#include "../scenes/battle/BattleScene.hpp"
#include "SDL_RectOps.hpp"
#include "SDL3/SDL_render.h"

Battle::PaletteRenderer::PaletteRenderer(BattleScene* scene)
	: scene(scene) {
}

void Battle::PaletteRenderer::process() {
	if (scene->getCurrentFighter() == FighterId::MaxFighters) { return; }
}