#include "TextDecisionScene.hpp"
#include "TextBoxScene.hpp"
#include "GRY_JSON.hpp"
#include "GRY_PixelGame.hpp"
#include "SDL_RectOps.hpp"
#include "SDL3/SDL_render.h"

static const float LINE_SPACING = 2.f;
static const float SPACE_FROM_TEXTBOX = 2.f;

void TextDecisionScene::setSelection(Selection selectionValue) {
	selectionStrings[selection][0] = ' ';
	selection = selectionValue;
	selectionStrings[selection][0] = '>';
}

void TextDecisionScene::setControls() {
	controls.mapCmd(GCmd::MenuOk, VirtualButton::GAME_A);
	controls.mapCmd(GCmd::MenuBack, VirtualButton::GAME_B);
	controls.mapCmd(GCmd::MenuUp, VirtualButton::GAME_UP);
	controls.mapCmd(GCmd::MenuDown, VirtualButton::GAME_DOWN);
}

TextDecisionScene::TextDecisionScene(GRY_PixelGame *pGame, const char *scenePath, TextBoxScene* scene) :
	Scene((GRY_Game*)pGame, scenePath),
	scene(scene),
	renderer(scene->getGame()->getVideo().getRenderer()),
	pixelScaling(&scene->getPixelGame()->getPixelScalingRef()) {
}

void TextDecisionScene::init() {
	setControls();
	
	float textureWidth;
	float textureHeight;
	SDL_GetTextureSize(boxTexture.texture, &textureWidth, &textureHeight);

	boxTextureArea = scene->getBoxTextureArea();
	boxTextureArea.x += boxTextureArea.w - textureWidth;
	boxTextureArea.y -= textureHeight + SPACE_FROM_TEXTBOX;
	boxTextureArea.w = textureWidth;
	boxTextureArea.h = textureHeight;
	

	/* Recall that textArea width and height have the margins stored in them right now */
	textArea.x = (int)boxTextureArea.x + textArea.w;
	textArea.y = (int)boxTextureArea.y + textArea.h;
	/* Now we can set the actual width and height */
	textArea.w = (int)boxTextureArea.w - (2 * textArea.w);
	textArea.h = (int)boxTextureArea.h - (2 * textArea.h);
}

void TextDecisionScene::process() {
	if (!active) { return; }
	GCmd cmd = readSingleInput();
	switch (cmd) {
		case GCmd::MenuOk:
			if (selection == NONE) { setSelection(Selection::YES); }
			else { decisionMade = true; }
			break;
		case GCmd::MenuBack:
			setSelection(Selection::NO);
			break;
		case GCmd::MenuUp:
		case GCmd::MenuDown:
			setSelection(selection == Selection::YES ? Selection::NO : Selection::YES);
			break;
		default:
			break;
	}

	const Fontset& font = scene->getFont();

	SDL_FRect boxDstRect = boxTextureArea;
	boxDstRect *= *pixelScaling;
	SDL_RenderTexture(renderer, boxTexture.texture, NULL, &boxDstRect);

	float cursorX = textArea.x;
	float cursorY = textArea.y;

	for (int i = 1; i < 3; i++) {
		for (char* c = selectionStrings[i]; *c; c++) {
			const SDL_FRect* srcRect = font.getSourceRect(*c - ' ');
			SDL_FRect dstRect { cursorX, cursorY, srcRect->w, srcRect->h };

			dstRect *= *pixelScaling;
			SDL_RenderTexture(renderer, font.texture, font.getSourceRect(*c - ' '), &dstRect);

			cursorX += srcRect->w;
		}
		cursorX = textArea.x;
		cursorY += font.charHeight + LINE_SPACING;
	}
}

bool TextDecisionScene::load() {
	if (boxTexture.path) {
		return boxTexture.load(game);
	}

	/* Open scene document */
	GRY_JSON::Document sceneDoc;
	GRY_JSON::loadDoc(sceneDoc, scenePath);

	/* Initialize the box texture */
	boxTexture.setPath(sceneDoc["boxTexturePath"].GetString());
	/* Store margins in textArea for now */
	textArea.w = sceneDoc["marginX"].GetUint();
	textArea.h = sceneDoc["marginY"].GetUint();
	return false;
}

void TextDecisionScene::open() {
	GRY_Assert(!active, "[DecisionBoxScene] open() called when the decision box was already open!");
	activateControlScheme();
	active = true;
	decisionMade = false;
	setSelection(Selection::NONE);
}

void TextDecisionScene::close() {
	GRY_Assert(active, "[DecisionBoxScene] close() called when the decision box was not open!");
	scene->activateControlScheme();
	active = false;
}
