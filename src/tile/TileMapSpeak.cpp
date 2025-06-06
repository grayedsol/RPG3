/**
 * @file TileMapSpeak.cpp
 * @author Grayedsol (grayedsol@gmail.com)
 * @copyright Copyright (c) 2025
 */
#include "TileMapSpeak.hpp"
#include "../scenes/TextBoxScene.hpp"
#include "../scenes/TileMapScene.hpp"

Tile::MapSpeak::MapSpeak(MapScene* scene) : scene(scene), textbox(&scene->getTextBox()) {
}

void Tile::MapSpeak::process() {
	if (!currentDialogue || !textbox->isReady()) { return; }

	if (index >= currentDialogue->size()) {
		textbox->close();
		currentDialogue = nullptr;
		index = 0;
	}
	else {
		textbox->printLine(currentDialogue->at(index));
		index++;
	}
}

void Tile::MapSpeak::speak(unsigned dialogueId) {
	if (textbox->isOpen()) { return; }
	currentDialogue = &scene->getDialogueResource().dialogues.at(dialogueId);
	textbox->open();
}
