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
		PlainDialogue message;
		Dialogue dialogue;
		for (auto& line : value["message"].GetArray()) {
			message.push_back(GRY_copyString(line.GetString()));
			dialogue.lines.push_back(GRY_copyString(line.GetString()));
		}
		dialogues.push_back(message);
		if (value.HasMember("branch")) {
			GRY_Assert(value["branch"].IsArray(), "[DialogueResource::load] \"branch\" value was not an array.\n");
			GRY_Assert(value["branch"].GetArray().Size() == 2, "[DialogueResource::load] \"branch\" value must be an array of size 2.\n");
			dialogue.branching = true;
			dialogue.path1 = value["branch"].GetArray()[0].GetUint();
			dialogue.path2 = value["branch"].GetArray()[1].GetUint();
		}
		dialogues2.push_back(dialogue);
	}

	/* Return false normally, but if there were no messages we can return true. */
	return dialogueDoc["data"].GetArray().Size() == 0;
}