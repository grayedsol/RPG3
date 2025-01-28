#include "TextBoxRenderer.hpp"
#include "../scenes/TextBoxScene.hpp"
#include "GRY_PixelGame.hpp"
#include "SDL_FRectOps.hpp"
#include <math.h>

static const float LINE_SPACING = 2.f;

struct TextCursor {
	float x = 0;
	float y = 0;
};

void TextBoxRenderer::printChar(char character, float x, float y) {
}

TextBoxRenderer::TextBoxRenderer(TextBoxScene *scene) : scene(scene),
	renderer(scene->getGame()->getSDL().getRenderer()),
	pixelScaling(&scene->getPixelGame()->getPixelScalingRef()) {
}

void TextBoxRenderer::process() {
	SDL_FRect dstRect = scene->getBoxTextureArea();
	dstRect *= *pixelScaling;
	SDL_RenderTexture(renderer, scene->getBoxTexture(), NULL, &dstRect);
	cursor.x = 0;
	cursor.y = yStart;
}

void TextBoxRenderer::beginProcess() {
	SDL_FRect dstRect = scene->getBoxTextureArea();
	dstRect *= *pixelScaling;
	SDL_RenderTexture(renderer, scene->getBoxTexture(), NULL, &dstRect);

	SDL_Rect rect = scene->getTextArea();
	rect *= (int)*pixelScaling;
	SDL_SetRenderViewport(renderer, &rect);

	cursor.x = 0;
	cursor.y = yStart;
}

void TextBoxRenderer::endProcess() {
	SDL_SetRenderViewport(renderer, NULL);
}

bool TextBoxRenderer::printLine(const char *line, double scrollSpeed, double delta, int index) {
	if (index < 0) { index = 256; } /**< TextBoxScene::MAX_LINE_LENGTH */
	
	const Fontset& font = scene->getFont();
	SDL_Rect rect = scene->getTextArea();
	
	for (int i = 0; i < index && line[i]; i++) {
		if (cursor.y + font.charHeight + LINE_SPACING > rect.h) {
			yStart -= scrollSpeed * delta;
			if (cursor.y - (scrollSpeed * delta) + font.charHeight + LINE_SPACING < rect.h) { 
				yStart += rect.h - cursor.y + (scrollSpeed * delta) - font.charHeight - LINE_SPACING;
			}
			return false;
		}
		if (line[i] == '\n') {
			cursor.y += font.charHeight + LINE_SPACING;
			cursor.x = 0;
		}
		else {
			const SDL_FRect* srcRect = font.getSourceRect(line[i] - ' ');
			SDL_FRect dstRect{
				cursor.x, cursor.y,
				srcRect->w, srcRect->h
			};
			cursor.x += dstRect.w;
			dstRect *= *pixelScaling;
			dstRect.y = ceilf(dstRect.y);
			SDL_RenderTexture(renderer, font.texture, srcRect, &dstRect);
		}
	}
	cursor.y += font.charHeight + LINE_SPACING;
	cursor.x = 0;
	return true;
}

void TextBoxRenderer::beginRender(const char* storedLine) {
	const Fontset& font = scene->getFont();

	yStart = scene->getTextArea().h - (font.charHeight + LINE_SPACING);
	for (; *storedLine != '\0'; storedLine++) {
		if (*storedLine == '\n') { yStart -= font.charHeight + LINE_SPACING; }
	}
	yStart = std::min(0.f, yStart);
}

void TextBoxRenderer::endRender() {
	cursor.x = 0;
	cursor.y = yStart;
}

void TextBoxRenderer::scrollUp(double scrollSpeed, double delta) {
	yStart -= scrollSpeed * delta;
}

float TextBoxRenderer::yAfterPrintingLine(const char *line) {
	float retVal = cursor.y;
	const Fontset& font = scene->getFont();

	for (; *line != '\0'; line++) {
		if (*line == '\n') { retVal += font.charHeight + LINE_SPACING; }
	}
	retVal += font.charHeight + LINE_SPACING;
	return retVal;
}
