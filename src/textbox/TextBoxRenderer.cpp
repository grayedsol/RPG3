#include "TextBoxRenderer.hpp"
#include "../scenes/TextBoxScene.hpp"
#include "GRY_PixelGame.hpp"

static const float BOTTOM_MARGIN = 8.f;

TextBoxRenderer::TextBoxRenderer(TextBoxScene *scene) :
	scene(scene),
	renderer(scene->getGame()->getSDL().getRenderer()),
	pixelScaling(&scene->getPixelGame()->getPixelScalingRef()) {
}

void TextBoxRenderer::init() {
	GRY_PixelGame* pGame = scene->getPixelGame();
	unsigned screenWidth = pGame->getScreenWidthPixels();
	unsigned screenHeight = pGame->getScreenHeightPixels();

	SDL_Texture* boxTexture = scene->getBoxTexture();
	float textureWidth;
	float textureHeight;
	SDL_GetTextureSize(boxTexture, &textureWidth, &textureHeight);

	float x = ((float)screenWidth - textureWidth) * 0.5f;
	float y = ((float)screenHeight - textureHeight - BOTTOM_MARGIN);

	dstRect = SDL_FRect{ x, y, textureWidth, textureHeight };
}

void TextBoxRenderer::process() {
	SDL_FRect scaledDstRect{
		dstRect.x * *pixelScaling, dstRect.y * *pixelScaling,
		dstRect.w * *pixelScaling, dstRect.h * *pixelScaling
	};
	SDL_RenderTexture(renderer, scene->getBoxTexture(), NULL, &scaledDstRect);
}
