#include "TextBoxRenderer.hpp"
#include "../scenes/TextBoxScene.hpp"
#include "GRY_PixelGame.hpp"

static const float BOTTOM_MARGIN = 8.f;

TextBoxRenderer::TextBoxRenderer(TextBoxScene* scene) :
	scene(scene),
	renderer(scene->getGame()->getSDL().getRenderer()),
	pixelScaling(&scene->getPixelGame()->getPixelScalingRef()) {
}

void TextBoxRenderer::process() {
	SDL_FRect dstRect = scene->getBoxTextureArea();
	SDL_FRect scaledDstRect{
		dstRect.x * *pixelScaling, dstRect.y * *pixelScaling,
		dstRect.w * *pixelScaling, dstRect.h * *pixelScaling
	};
	SDL_RenderTexture(renderer, scene->getBoxTexture(), NULL, &scaledDstRect);
}
