#include "MenuScene.hpp"
#include "GRY_PixelGame.hpp"
#include "GRY_JSON.hpp"
#include "SDL3/SDL_render.h"
#include "SDL_RectOps.hpp"

static const float LEFT_MARGIN = 8.f;
static const float TOP_MARGIN = 8.f;
static const float LINE_SPACING = 2.f;
static const float COLUMN_SPACING = 64.f;

static const char* menuStrings[7] = {
	" ",
	" Item", " Spells & Skills",
	" Equip", " Records",
	" Status", " Misc."
};

void MenuScene::setControls() {
	controls.mapCmd(GCmd::MenuOk, VirtualButton::GAME_A);
	controls.mapCmd(GCmd::MenuBack, VirtualButton::GAME_B);
	controls.mapCmd(GCmd::MenuUp, VirtualButton::GAME_UP);
	controls.mapCmd(GCmd::MenuDown, VirtualButton::GAME_DOWN);
	controls.mapCmd(GCmd::MenuLeft, VirtualButton::GAME_LEFT);
	controls.mapCmd(GCmd::MenuRight, VirtualButton::GAME_RIGHT);
}

void MenuScene::setSelection(uint8_t selectionValue) {
	renderInfo.selectionStrings[selection][0] = ' ';
	renderInfo.selectionStrings[selectionValue][0] = '>';
	selection = selectionValue;
}

void MenuScene::renderMenu(const MenuRenderInfo& info) {
	SDL_FRect boxDstRect = info.boxTextureArea;
	boxDstRect *= *pixelScaling;
	SDL_RenderTexture(renderer, renderInfo.boxTexture.texture, NULL, &boxDstRect);

	float cursorX = info.textArea.x;
	float cursorY = info.textArea.y;

	for (int i = 0; i < info.numRows; i++) {
		for (int j = 0; j < info.numCols; j++) {
			for (char* c = info.selectionStrings[i * info.numCols + j]; *c; c++) {
				const SDL_FRect* srcRect = font.getSourceRect(*c - ' ');
				SDL_FRect dstRect { cursorX, cursorY, srcRect->w, srcRect->h };

				dstRect *= *pixelScaling;
				SDL_RenderTexture(renderer, font.texture, font.getSourceRect(*c - ' '), &dstRect);

				cursorX += srcRect->w;
			}
			cursorX = info.textArea.x + COLUMN_SPACING;
		}
		cursorX = info.textArea.x;
		cursorY += font.charHeight + LINE_SPACING;
	}
}

MenuScene::MenuScene(GRY_PixelGame* pGame, const char* path, Scene* scene) :
	Scene((GRY_Game*)pGame, path),
	scene(scene),
	renderer(pGame->getVideo().getRenderer()),
	pixelScaling(&pGame->getPixelScalingRef()) {
}

MenuScene::~MenuScene() {
	for (int i = 0; i < renderInfo.numRows * renderInfo.numCols; i++) {
		delete[] renderInfo.selectionStrings[i];
	}
	delete[] renderInfo.selectionStrings;
}

void MenuScene::init() {
	setControls();
	
	float textureWidth;
	float textureHeight;
	SDL_GetTextureSize(renderInfo.boxTexture.texture, &textureWidth, &textureHeight);

	float x = LEFT_MARGIN;
	float y = TOP_MARGIN;

	renderInfo.boxTextureArea = SDL_FRect { x, y, textureWidth, textureHeight };

	/* Recall that renderInfo.textArea width and height have the margins stored in them right now */
	renderInfo.textArea.x = (int)renderInfo.boxTextureArea.x + renderInfo.textArea.w;
	renderInfo.textArea.y = (int)renderInfo.boxTextureArea.y + renderInfo.textArea.h;
	/* Now we can set the actual width and height */
	renderInfo.textArea.w = (int)renderInfo.boxTextureArea.w - (2 * renderInfo.textArea.w);
	renderInfo.textArea.h = (int)renderInfo.boxTextureArea.h - (2 * renderInfo.textArea.h);
}

void MenuScene::process() {
	if (!active) { return; }

	int currentSelection = selection;
	switch (readSingleInput()) {
		case GCmd::MenuOk:
			break;
		case GCmd::MenuBack:
			close();
			return;
		case GCmd::MenuUp:
			currentSelection -= renderInfo.numCols;
			if (currentSelection < 0) { currentSelection += renderInfo.numRows * renderInfo.numCols; }
			setSelection((uint8_t)currentSelection);
			break;
		case GCmd::MenuDown:
			currentSelection += renderInfo.numCols;
			if (currentSelection >= renderInfo.numRows * renderInfo.numCols) { currentSelection -= renderInfo.numRows * renderInfo.numCols; }
			setSelection((uint8_t)currentSelection);
			break;
		case GCmd::MenuLeft:
			currentSelection += currentSelection % renderInfo.numCols ? -1 : (renderInfo.numCols - 1);
			setSelection(currentSelection);
			break;
		case GCmd::MenuRight:
			currentSelection += (currentSelection + 1) % renderInfo.numCols ? 1 : -(renderInfo.numCols - 1);
			setSelection((uint8_t)currentSelection);
			break;
		default:
			break;
	}

	renderMenu(renderInfo);
}

bool MenuScene::load() {
	if (renderInfo.boxTexture.path && font.path) {
		return renderInfo.boxTexture.load(game) && font.load(game);
	}

	/* Open scene document */
	GRY_JSON::Document sceneDoc;
	GRY_JSON::loadDoc(sceneDoc, scenePath);

	/* Initialize the box texture */
	renderInfo.boxTexture.setPath(sceneDoc["boxTexturePath"].GetString());
	/* Initialize the font texture */
	font.setPath(sceneDoc["fontTexturePath"].GetString());
	/* Initialize row and column counts */
	renderInfo.numRows = sceneDoc["numRows"].GetUint();
	renderInfo.numCols = sceneDoc["numCols"].GetUint();
	/* Initialize selection strings */
	auto& strings = sceneDoc["selectionStrings"];
	GRY_Assert(strings.GetArray().Size() == renderInfo.numCols * renderInfo.numRows,
		"[MenuScene] The number of selection strings must be equal to the product of the number of rows and columns.\n"
	);
	renderInfo.selectionStrings = new char*[strings.GetArray().Size()];
	for (int i = 0; i < strings.GetArray().Size(); i++) {
		const char* string = strings.GetArray()[i].GetString();
		renderInfo.selectionStrings[i] = new char[strlen(string) + 2];
		renderInfo.selectionStrings[i][0] = ' ';
		for (int j = 0; j < strlen(string); j++) {
			renderInfo.selectionStrings[i][j+1] = string[j];
		}
		renderInfo.selectionStrings[i][strlen(string)+1] = '\0';
	}
	renderInfo.selectionStrings[0][0] = '>';
	/* Store margins in renderInfo.textArea for now */
	renderInfo.textArea.w = sceneDoc["marginX"].GetUint();
	renderInfo.textArea.h = sceneDoc["marginY"].GetUint();

	return false;
}

void MenuScene::open() {
	GRY_Assert(!active, "[MenuScene] open() called when the decision box was already open!");
	activateControlScheme();
	active = true;
}

void MenuScene::close() {
	GRY_Assert(active, "[MenuScene] close() called when the decision box was not open!");
	scene->activateControlScheme();
	active = false;
	setSelection(0);
}
