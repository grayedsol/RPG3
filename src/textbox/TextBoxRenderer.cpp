#include "TextBoxRenderer.hpp"
#include "../scenes/TextBoxScene.hpp"
#include "GRY_PixelGame.hpp"
#include "SDL_FRectOps.hpp"

static const float BOTTOM_MARGIN = 8.f;

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
}

void TextBoxRenderer::printLine(const char *line) {
	const Fontset& font = scene->getFont();
	SDL_FRect textArea = scene->getTextArea();
	float x = textArea.x;
	float y = textArea.y;
	for (; *line != '\0'; line++) {
		if (*line == '\n') { y += font.charHeight + 2; x = textArea.x; }
		else {
			const SDL_FRect* srcRect = font.getSourceRect(*line - ' ');
			SDL_FRect dstRect{
				x, y,
				srcRect->w, srcRect->h 
			};
			x += dstRect.w;
			dstRect *= *pixelScaling;
			SDL_RenderTexture(renderer, font.texture, srcRect, &dstRect);
		}
	}
}
