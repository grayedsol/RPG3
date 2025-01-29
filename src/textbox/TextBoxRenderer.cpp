#include "TextBoxRenderer.hpp"
#include "../scenes/TextBoxScene.hpp"
#include "GRY_PixelGame.hpp"
#include "SDL_FRectOps.hpp"
#include <math.h>

static const float LINE_SPACING = 2.f;

TextBoxRenderer::TextBoxRenderer(TextBoxScene *scene) : scene(scene),
	renderer(scene->getGame()->getSDL().getRenderer()),
	pixelScaling(&scene->getPixelGame()->getPixelScalingRef()) {
}

void TextBoxRenderer::beginRender() {
	SDL_FRect dstRect = scene->getBoxTextureArea();
	dstRect *= *pixelScaling;
	SDL_RenderTexture(renderer, scene->getBoxTexture(), NULL, &dstRect);

	SDL_Rect rect = scene->getTextArea();
	rect *= (int)*pixelScaling;
	SDL_SetRenderViewport(renderer, &rect);

	cursor.x = 0;
	cursor.y = yStart;
}

void TextBoxRenderer::endRender() {
	SDL_SetRenderViewport(renderer, NULL);
}

bool TextBoxRenderer::printLine(const char* line, double scrollAmt, int index) {
	if (!*line) { return false; }
	if (index < 0) { index = TextBoxScene::MAX_LINE_LENGTH; }
	
	const Fontset& font = scene->getFont();
	SDL_Rect rect = scene->getTextArea();
	double lineSpaceAvailable = rect.h - font.charHeight - LINE_SPACING;

	for (int i = 0; i < index && line[i]; i++) {
		if (lineSpaceAvailable < cursor.y) {
			yStart += std::max(lineSpaceAvailable - cursor.y, -scrollAmt);
			return false;
		}
		if (line[i] == '\n') {
			cursor.y += font.charHeight + LINE_SPACING;
			cursor.x = 0;
			continue;
		}

		const SDL_FRect* srcRect = font.getSourceRect(line[i] - ' ');

		if (cursor.x + srcRect->w > rect.w) {
			cursor.y += font.charHeight + LINE_SPACING;
			cursor.x = 0;
			i--;
			continue;
		}

		SDL_FRect dstRect{
			cursor.x, cursor.y,
			srcRect->w, srcRect->h
		};
		dstRect *= *pixelScaling;
		dstRect.y = floorf(dstRect.y);

		cursor.x += srcRect->w;
		SDL_RenderTexture(renderer, font.texture, srcRect, &dstRect);
	}
	cursor.y += font.charHeight + LINE_SPACING;
	cursor.x = 0;
	return true;
}

void TextBoxRenderer::setSpacingFromLine(const char* storedLine) {
	const Fontset& font = scene->getFont();

	yStart = scene->getTextArea().h - (font.charHeight + LINE_SPACING);
	for (; *storedLine != '\0'; storedLine++) {
		if (*storedLine == '\n') { yStart -= font.charHeight + LINE_SPACING; }
	}
	yStart = std::min(0.f, yStart);
	cursor.y = yStart;
}
