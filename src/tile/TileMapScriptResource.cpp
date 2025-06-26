#include "TileMapScriptResource.hpp"
#include "TileRegisterMapCommandFuncs.hpp"
#include "GRY_JSON.hpp"

bool Tile::MapScriptResource::load(GRY_Game *game) {
	if (!scripts.empty()) { return true; }

	/* Open the script file */
	GRY_JSON::Document scriptDoc;
	GRY_JSON::loadDoc(scriptDoc, path);

	float normalTileSize = scriptDoc["normalTileSize"].GetFloat();

	/* Load scripts */
	for (auto& scriptData : scriptDoc["scripts"].GetArray()) {
		MapCommandScript script;
		for (auto& commandSetData : scriptData["script"].GetArray()) {
			MapCommandSet commandSet;
			for (auto& commandData : commandSetData["commandSet"].GetArray()) {
				for (int i = 0; i < MapCommandType::MAP_CMD_SIZE; i++) {
					if (strcmp(commandData["type"].GetString(), MapCommandNames[i]) == 0) {
						MapCommand command = registerTMC_Funcs[i](normalTileSize, commandData, ECS::NONE);
						commandSet.push_back(command);
						break;
					}
				}
			}
			script.push_back(commandSet);
		}
		scripts.push_back(script);
	}

	return scriptDoc["scripts"].GetArray().Size() == 0;
}