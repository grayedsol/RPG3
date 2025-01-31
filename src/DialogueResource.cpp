/**
 * @file DialogueResource.cpp
 * @author Grayedsol (grayedsol@gmail.com)
 * @copyright Copyright (c) 2025
 */
#include "DialogueResource.hpp"
#include "GRY_Game.hpp"
#include "GRY_JSON.hpp"

bool DialogueResource::load(GRY_Game* game) {
	if (!dialogues.empty()) { return true; }
	GRY_JSON::Document dialogueDoc;
	GRY_JSON::loadDoc(dialogueDoc, path);

	for (auto& value : dialogueDoc["data"].GetArray()) {
		Dialogue message;
		for (auto& line : value["message"].GetArray()) {
			message.push_back(GRY_copyString(line.GetString()));
		}
		dialogues.push_back(message);
	}

	/* Return false normally, but if there were no messages we can return true. */
	return dialogueDoc["data"].GetArray().Size() == 0;
}