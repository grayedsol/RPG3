/**
 * @file TileMapSpeak.hpp
 * @author Grayedsol (grayedsol@gmail.com)
 * @brief @copybrief Tile::MapSpeak
 * @copyright Copyright (c) 2025
 */
#pragma once
#include "TileMapDialogueResource.hpp"

class TextBoxScene;

namespace Tile {
	class MapScene;

	class MapSpeak {
	private:
		const MapDialogue* currentDialogue = nullptr;

		TextBoxScene* textbox;

		unsigned index = 0;

		MapScene* scene;

		void endSpeak();
	public:
		MapSpeak(MapScene* scene);

		void process();

		void speak(unsigned dialogueId);
	};
};