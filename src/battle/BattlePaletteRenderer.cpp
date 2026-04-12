#include "BattlePaletteRenderer.hpp"
#include "../scenes/battle/BattleScene.hpp"
#include "GRY_PixelGame.hpp"
#include "SDL_RectOps.hpp"
#include "SDL3/SDL_render.h"

static const char* paletteBoxStrings[5] = {
	"Attack",
	"Item",
	"Skill",
	"Wait",
	"Main"
};

Battle::PaletteRenderer::PaletteRenderer(BattleScene* scene)
	: scene(scene)
	, pixelScaling(&scene->getPixelGame()->getPixelScalingRef()) {
}

void Battle::PaletteRenderer::init() {
	renderer = scene->getGame()->getVideo().getRenderer();
	paletteBoxTexture = &scene->getTexture(BattleSceneTextureIndex::TEXTURE_INDEX_PALETTE_BOX);
	renderW = paletteBoxTexture->texture->w;
	renderH = paletteBoxTexture->texture->h;

	const Fontset& font = scene->getFont();
	
	textYOffset = (renderH - font.charHeight + font.underLineHeight) * 0.5f;
}

void Battle::PaletteRenderer::process() {
	if (scene->getCurrentFighter() == FighterId::MaxFighters) { return; }

	const Fontset& font = scene->getFont();

	const char* strings[numTexts] = { 0 };
	float startX[numTexts] = { 0 };
	/* Get text depending on which page we're on */
	switch (scene->getCurrentPage()) {
	case PageId::MainPage:
		for (int i = 0; i < numBoxes; i++) { strings[i] = paletteBoxStrings[i]; }
		break;
	case PageId::ItemPage:
		for (int i = 0; i < numBoxes; i++) { strings[i] = "ItemTemp"; }
		strings[2] = paletteBoxStrings[4];
		break;
	case PageId::SkillPage:
	for (int i = 0; i < numBoxes; i++) { strings[i] = "SkillTemp"; }
		strings[1] = paletteBoxStrings[4];
		break;
	}
	strings[numTexts - 1] = scene->getActors().names[scene->getCurrentFighter()];

	/* Calculate the starting point of each text by finding the total width and centering */
	for (int i = 0; i < numTexts; i++) {
		float wordWidthPixels = 0.f;
		for (const char* c = strings[i]; *c; c++) {
			const SDL_FRect* srcRect = font.getSourceRect(*c - ' ');
			wordWidthPixels += srcRect->w;
		}
		/* Add 1 here to account for extra ending width of last character */
		startX[i] = SDL_ceilf((renderW - wordWidthPixels + 1) * 0.5f);
	}

	/* Render the palette boxes */
	for (int i = 0; i < numBoxes; i++) {
		SDL_FRect dstRect {
			renderX[i], renderY[i], renderW, renderH
		};
		dstRect *= *pixelScaling;

		SDL_RenderTexture(renderer, paletteBoxTexture->texture, NULL, &dstRect);
	}

	/* Render the palette box text */
	for (int i = 0; i < numTexts; i++) {
		SDL_FRect dstRect {
			(renderX[i] + startX[i]) * *pixelScaling,
			(renderY[i] + textYOffset) * *pixelScaling
		};
		for (const char* c = strings[i]; *c; c++) {
			const SDL_FRect* srcRect = font.getSourceRect(*c - ' ');
			dstRect.w = srcRect->w * *pixelScaling;
			dstRect.h = srcRect->h * *pixelScaling;
			SDL_RenderTexture(renderer, font.texture, srcRect, &dstRect);
			dstRect.x += dstRect.w;
		}
	}
}

void Battle::PaletteRenderer::setRenderPos(unsigned index, float x, float y) {
	renderX[index] = x;
	renderY[index] = y;
}
