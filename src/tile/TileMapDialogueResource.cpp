/**
 * @file TileMapDialogueResource.cpp
 * @author Grayedsol (grayedsol@gmail.com)
 * @copyright Copyright (c) 2025
 */
#include "TileMapDialogueResource.hpp"
#include "TileRegisterMapCommandFuncs.hpp"
#include "GRY_Game.hpp"
#include "GRY_JSON.hpp"

bool Tile::MapDialogueResource::load(GRY_Game* game) {
	if (!dialogues.empty()) { return true; }
	GRY_JSON::Document dialogueDoc;
	GRY_JSON::loadDoc(dialogueDoc, path);

	float normalTileSize = dialogueDoc["normalTileSize"].GetFloat();

	for (auto& value : dialogueDoc["data"].GetArray()) {
		MapDialogue dialogue;
		for (auto& line : value["message"].GetArray()) {
			dialogue.lines.push_back(GRY_copyString(line.GetString()));
		}
		if (value.HasMember("branch")) {
			GRY_Assert(value["branch"].IsArray(), "[MapDialogueResource::load] \"branch\" value was not an array.\n");
			GRY_Assert(value["branch"].GetArray().Size() == 2, "[MapDialogueResource::load] \"branch\" value must be an array of size 2.\n");
			dialogue.branching = true;
			dialogue.path1 = value["branch"].GetArray()[0].GetUint();
			dialogue.path2 = value["branch"].GetArray()[1].GetUint();
		}
		if (value.HasMember("command")) {
			for (int i = 0; i < MapCommandType::MAP_CMD_SIZE; i++) {
				if (strcmp(value["command"]["type"].GetString(), MapCommandNames[i]) == 0) {
					dialogue.command = registerTMC_Funcs[i](normalTileSize, value["command"], ECS::NONE);
					break;
				}
			}
		}
		dialogues.push_back(dialogue);
	}

	/* Return false normally, but if there were no messages we can return true. */
	return dialogueDoc["data"].GetArray().Size() == 0;
}