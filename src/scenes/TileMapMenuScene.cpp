#include "TileMapMenuScene.hpp"
#include "TileMapScene.hpp"
#include "GRY_PixelGame.hpp"
#include "GRY_JSON.hpp"
#include "SDL3/SDL_render.h"
#include "SDL_RectOps.hpp"

static const float LEFT_MARGIN = 8.f;
static const float TOP_MARGIN = 8.f;
static const float LINE_SPACING = 2.f;
static const float COLUMN_SPACING = 64.f;

void Tile::MapMenuScene::setControls() {
	controls.mapCmd(GCmd::MenuOk, VirtualButton::GAME_A);
	controls.mapCmd(GCmd::MenuBack, VirtualButton::GAME_B);
	controls.mapCmd(GCmd::MenuUp, VirtualButton::GAME_UP);
	controls.mapCmd(GCmd::MenuDown, VirtualButton::GAME_DOWN);
	controls.mapCmd(GCmd::MenuLeft, VirtualButton::GAME_LEFT);
	controls.mapCmd(GCmd::MenuRight, VirtualButton::GAME_RIGHT);
}

void Tile::MapMenuScene::setSelection(Selection selectionValue) {
	selectionStrings[static_cast<int8_t>(selection)][0] = ' ';
	selectionStrings[static_cast<int8_t>(selectionValue)][0] = '>';
	selection = selectionValue;
}

void Tile::MapMenuScene::renderMenu() {
	SDL_FRect boxDstRect = boxTextureArea;
	boxDstRect *= *pixelScaling;
	SDL_RenderTexture(renderer, boxTexture.texture, NULL, &boxDstRect);

	float cursorX = textArea.x;
	float cursorY = textArea.y;

	const int numRows = 3;
	const int numCols = 2;
	for (int i = 0; i < numRows; i++) {
		for (int j = 0; j < numCols; j++) {
			for (char* c = selectionStrings[i * numCols + j + 1]; *c; c++) {
				const SDL_FRect* srcRect = font.getSourceRect(*c - ' ');
				SDL_FRect dstRect { cursorX, cursorY, srcRect->w, srcRect->h };

				dstRect *= *pixelScaling;
				SDL_RenderTexture(renderer, font.texture, font.getSourceRect(*c - ' '), &dstRect);

				cursorX += srcRect->w;
			}
			cursorX = textArea.x + COLUMN_SPACING;
		}
		cursorX = textArea.x;
		cursorY += font.charHeight + LINE_SPACING;
	}
}

Tile::MapMenuScene::MapMenuScene(GRY_PixelGame* pGame, const char* path, MapScene* scene) :
	Scene((GRY_Game*)pGame, path),
	scene(scene),
	renderer(scene->getGame()->getVideo().getRenderer()),
	pixelScaling(&scene->getPixelGame()->getPixelScalingRef()) {
}

void Tile::MapMenuScene::init() {
	setControls();

	// GRY_PixelGame* pGame = (GRY_PixelGame*)game;
	// unsigned screenWidth = pGame->getScreenWidthPixels();
	// unsigned screenHeight = pGame->getScreenHeightPixels();
	
	float textureWidth;
	float textureHeight;
	SDL_GetTextureSize(boxTexture.texture, &textureWidth, &textureHeight);

	float x = LEFT_MARGIN;
	float y = TOP_MARGIN;

	boxTextureArea = SDL_FRect { x, y, textureWidth, textureHeight };

	/* Recall that textArea width and height have the margins stored in them right now */
	textArea.x = (int)boxTextureArea.x + textArea.w;
	textArea.y = (int)boxTextureArea.y + textArea.h;
	/* Now we can set the actual width and height */
	textArea.w = (int)boxTextureArea.w - (2 * textArea.w);
	textArea.h = (int)boxTextureArea.h - (2 * textArea.h);
}

void Tile::MapMenuScene::process() {
	if (!active) { return; }

	int8_t currentSelection = static_cast<int8_t>(selection);
	switch (readSingleInput()) {
		case GCmd::MenuOk:
			break;
		case GCmd::MenuBack:
			close();
			return;
		case GCmd::MenuUp:
			currentSelection -= 2;
			if (currentSelection <= 0) { currentSelection += 6; }
			setSelection(static_cast<Selection>(currentSelection));
			break;
		case GCmd::MenuDown:
			currentSelection += 2;
			if (currentSelection >= 7) { currentSelection -= 6; }
			setSelection(static_cast<Selection>(currentSelection));
			break;
		case GCmd::MenuLeft:
		case GCmd::MenuRight:
			currentSelection += currentSelection & 1 ? 1 : -1;
			setSelection(static_cast<Selection>(currentSelection));
			break;
		default:
			break;
	}

	renderMenu();
}

bool Tile::MapMenuScene::load() {
	if (boxTexture.path && font.path) {
		return boxTexture.load(game) && font.load(game);
	}

	/* Open scene document */
	GRY_JSON::Document sceneDoc;
	GRY_JSON::loadDoc(sceneDoc, scenePath);

	/* Initialize the box texture */
	boxTexture.setPath(sceneDoc["boxTexturePath"].GetString());
	/* Initialize the font texture */
	font.setPath(sceneDoc["fontTexturePath"].GetString());
	/* Store margins in textArea for now */
	textArea.w = sceneDoc["marginX"].GetUint();
	textArea.h = sceneDoc["marginY"].GetUint();

	return false;
}

void Tile::MapMenuScene::open() {
	GRY_Assert(!active, "[Tile::MapMenuScene] open() called when the decision box was already open!");
	activateControlScheme();
	active = true;
}

void Tile::MapMenuScene::close() {
	GRY_Assert(active, "[Tile::MapMenuScene] close() called when the decision box was not open!");
	scene->activateControlScheme();
	active = false;
	setSelection(Selection::Item);
}
