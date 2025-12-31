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
		MapScene* scene;
		
		TextBoxScene* textbox;

		const MapDialogueResource* dialogueResource;

		const MapDialogue* currentDialogue = nullptr;

		unsigned index = 0;

		void endSpeak();
	public:
		MapSpeak(MapScene* scene, TextBoxScene* textboxScene, const MapDialogueResource* dialogueResource);

		void process();

		void speak(unsigned dialogueId);
	};
};