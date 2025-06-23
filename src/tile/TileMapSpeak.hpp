/**
 * @file TileMapSpeak.hpp
 * @author Grayedsol (grayedsol@gmail.com)
 * @brief @copybrief Tile::MapSpeak
 * @copyright Copyright (c) 2025
 */
#pragma once
#include "DialogueResource.hpp"

class TextBoxScene;

namespace Tile {
	class MapScene;

	class MapSpeak {
	private:
		const Dialogue* currentDialogue = nullptr;

		TextBoxScene* textbox;

		unsigned index = 0;

		MapScene* scene;
	public:
		MapSpeak(MapScene* scene);

		void process();

		void speak(unsigned dialogueId);
	};
};