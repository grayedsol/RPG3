/**
 * @file TileMapSpeak.cpp
 * @author Grayedsol (grayedsol@gmail.com)
 * @copyright Copyright (c) 2025
 */
#include "TileMapSpeak.hpp"
#include "../scenes/TextBoxScene.hpp"
#include "../scenes/TileMapScene.hpp"

void Tile::MapSpeak::endSpeak() {
	index = 0;
	scene->getECS().getComponent<Player>().value.at(0).speakingTo = ECS::NONE;
	if (currentDialogue->command.data.type != MAP_CMD_NONE) {
		MapCommand command = currentDialogue->command;
		scene->executeCommand(command);
		if (command.data.type == MAP_CMD_ACTOR_SPEAK) { return; }
	}
	textbox->close();
	currentDialogue = nullptr;
}

Tile::MapSpeak::MapSpeak(MapScene *scene) : scene(scene), textbox(&scene->getTextBox()) {
}

void Tile::MapSpeak::process() {
	if (!currentDialogue || !textbox->isReady()) { return; }

	if (index >= currentDialogue->lines.size()) {
		if (currentDialogue->branching) {
			if (!textbox->decisionBoxIsOpen()) { textbox->openDecisionBox(); }
			else if (textbox->decisionIsMade()) {
				textbox->closeDecisionBox();
				unsigned int dialogueId = textbox->getDecision() == 1 ? currentDialogue->path1 : currentDialogue->path2;
				currentDialogue = &scene->getDialogueResource().dialogues.at(dialogueId);
				index = 0;
				if (!currentDialogue->lines.size()) { endSpeak(); }
				else {
					textbox->printLine(currentDialogue->lines.at(index));
					index++;
				}
			}
		}
		else if (scene->readSingleInput() == GCmd::MessageOk || index == 0) {
			endSpeak();
		}
	}
	else if (scene->readSingleInput() == GCmd::MessageOk || index == 0) {
		textbox->printLine(currentDialogue->lines.at(index));
		index++;
	}
}

void Tile::MapSpeak::speak(unsigned dialogueId) {
	currentDialogue = &scene->getDialogueResource().dialogues.at(dialogueId);
	if (textbox->isOpen()) { return; }
	textbox->open();
}
