#include "MenuScene.hpp"
#include "GRY_PixelGame.hpp"
#include "GRY_JSON.hpp"
#include "SDL3/SDL_render.h"
#include "SDL_RectOps.hpp"

static const float LEFT_MARGIN = 8.f;
static const float TOP_MARGIN = 8.f;
static const float LINE_SPACING = 2.f;
static const float COLUMN_SPACING = 64.f;

void MenuScene::setControls() {
	controls.mapCmd(GCmd::MenuOk, VirtualButton::GAME_A);
	controls.mapCmd(GCmd::MenuBack, VirtualButton::GAME_B);
	controls.mapCmd(GCmd::MenuUp, VirtualButton::GAME_UP);
	controls.mapCmd(GCmd::MenuDown, VirtualButton::GAME_DOWN);
	controls.mapCmd(GCmd::MenuLeft, VirtualButton::GAME_LEFT);
	controls.mapCmd(GCmd::MenuRight, VirtualButton::GAME_RIGHT);
}

void MenuScene::setSelection(uint8_t selectionValue) {
	selectionStrings[selection][0] = ' ';
	selectionStrings[selectionValue][0] = '>';
	selection = selectionValue;
}

void MenuScene::renderMenu(const Fontset& font) {
	SDL_Renderer* renderer = getGame()->getVideo().getRenderer();
	const float* pixelScaling = &((GRY_PixelGame*)game)->getPixelScalingRef();

	SDL_FRect boxDstRect = boxTextureArea;
	boxDstRect *= *pixelScaling;
	SDL_RenderTexture(renderer, boxTexture.texture, NULL, &boxDstRect);

	float cursorX = textArea.x;
	float cursorY = textArea.y;

	for (int i = 0; i < numRows; i++) {
		for (int j = 0; j < numCols; j++) {
			for (char* c = selectionStrings[i * numCols + j]; *c; c++) {
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

MenuScene::MenuScene(GRY_PixelGame* pGame, const char* path, Scene* scene) :
	Scene((GRY_Game*)pGame, path),
	parentScene(scene) {
}

MenuScene::~MenuScene() {
	for (int i = 0; i < numRows * numCols; i++) {
		delete[] selectionStrings[i];
	}
	delete[] selectionStrings;
}

void MenuScene::init() {
	setControls();
	
	float textureWidth;
	float textureHeight;
	SDL_GetTextureSize(boxTexture.texture, &textureWidth, &textureHeight);

	boxTextureArea.w = textureWidth;
	boxTextureArea.h = textureHeight;

	/* Recall that textArea width and height have the margins stored in them right now */
	textArea.x = (int)boxTextureArea.x + textArea.w;
	textArea.y = (int)boxTextureArea.y + textArea.h;
	/* Now we can set the actual width and height */
	textArea.w = (int)boxTextureArea.w - (2 * textArea.w);
	textArea.h = (int)boxTextureArea.h - (2 * textArea.h);
}

bool MenuScene::load() {
	if (boxTexture.path) {
		return boxTexture.load(game);
	}

	/* Open scene document */
	GRY_JSON::Document sceneDoc;
	GRY_JSON::loadDoc(sceneDoc, scenePath);

	/* Initialize the box texture */
	boxTexture.setPath(sceneDoc["boxTexturePath"].GetString());
	/* Initialize row and column counts */
	numRows = sceneDoc["numRows"].GetUint();
	numCols = sceneDoc["numCols"].GetUint();
	/* Initialize selection strings */
	auto& strings = sceneDoc["selectionStrings"];
	GRY_Assert(strings.GetArray().Size() == numCols * numRows,
		"[MenuScene] The number of selection strings must be equal to the product of the number of rows and columns.\n"
	);
	selectionStrings = new char*[strings.GetArray().Size()];
	for (int i = 0; i < strings.GetArray().Size(); i++) {
		const char* string = strings.GetArray()[i].GetString();
		selectionStrings[i] = new char[strlen(string) + 2];
		selectionStrings[i][0] = ' ';
		for (int j = 0; j < strlen(string); j++) {
			selectionStrings[i][j+1] = string[j];
		}
		selectionStrings[i][strlen(string)+1] = '\0';
	}
	selectionStrings[0][0] = '>';
	/* Get position of the box */
	boxTextureArea.x = sceneDoc["positionX"].GetUint();
	boxTextureArea.y = sceneDoc["positionY"].GetUint();
	/* Store margins in textArea for now */
	textArea.w = sceneDoc["marginX"].GetUint();
	textArea.h = sceneDoc["marginY"].GetUint();

	return false;
}

void MenuScene::open() {
	GRY_Assert(!active, "[MenuScene] open() called when the decision box was already open!");
	activateControlScheme();
	active = true;
}

void MenuScene::close() {
	GRY_Assert(active, "[MenuScene] close() called when the decision box was not open!");
	parentScene->activateControlScheme();
	active = false;
	setSelection(0);
}

bool MenuScene::handleInput() {
	int currentSelection = selection;
	switch (readSingleInput()) {
		case GCmd::MenuOk:
			makeSelection(selection);
			break;
		case GCmd::MenuBack:
			close();
			return false;
		case GCmd::MenuUp:
			currentSelection -= numCols;
			if (currentSelection < 0) { currentSelection += numRows * numCols; }
			setSelection((uint8_t)currentSelection);
			break;
		case GCmd::MenuDown:
			currentSelection += numCols;
			if (currentSelection >= numRows * numCols) { currentSelection -= numRows * numCols; }
			setSelection((uint8_t)currentSelection);
			break;
		case GCmd::MenuLeft:
			currentSelection += currentSelection % numCols ? -1 : (numCols - 1);
			setSelection((uint8_t)currentSelection);
			break;
		case GCmd::MenuRight:
			currentSelection += (currentSelection + 1) % numCols ? 1 : -(numCols - 1);
			setSelection((uint8_t)currentSelection);
			break;
		default:
			break;
	}
	return true;
}
