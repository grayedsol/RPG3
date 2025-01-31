#include "TileMapSpeak.hpp"
#include "../scenes/TextBoxScene.hpp"
#include "../scenes/TileMapScene.hpp"

TileMapSpeak::TileMapSpeak(TileMapScene* scene) : scene(scene), textbox(&scene->getTextBox()) {
}

void TileMapSpeak::process() {
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

void TileMapSpeak::speak(unsigned dialogueId) {
	if (textbox->isOpen()) { return; }
	currentDialogue = &scene->getDialogueResource().dialogues.at(dialogueId);
	textbox->open();
}
