/**
 * @file TileMapSpeak.hpp
 * @author Grayedsol (grayedsol@gmail.com)
 * @brief @copybrief TileMapSpeak
 * @copyright Copyright (c) 2025
 */
#pragma once
#include "DialogueResource.hpp"

class TileMapScene;
class TextBoxScene;

class TileMapSpeak {
private:
	using Dialogue = DialogueResource::Dialogue;

	const Dialogue* currentDialogue = nullptr;

	TextBoxScene* textbox;

	int index = 0;

	TileMapScene* scene;
public:
	TileMapSpeak(TileMapScene* scene);

	void process();

	void speak(unsigned dialogueId);
};