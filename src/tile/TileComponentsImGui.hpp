/**
 * @file TileComponentsImGui.hpp
 * @author Grayedsol (grayedsol@gmail.com)
 * @brief Functions for displaying Tile Component data for Dear ImGui.
 * @copyright Copyright (c) 2025
 */
#pragma once
#include "TileComponents.hpp"
#include "GRY_ECSImGui.hpp"

template<>
void componentImGui(Tile::Actor& actor) {
	ImGui::Text("speed");
	ImGui::SameLine();
	ImGui::InputFloat("##0", &actor.speed, 1.f, 0);
	
	ImGui::Text("direction: %d", actor.direction);
	ImGui::Text("movingDirection: %d", actor.movingDirection);
	ImGui::Text("sprinting: %d", actor.sprinting);
}

template<>
void componentImGui(Tile::ActorSprite& sprite) {
	ImGui::Text("offset");
	ImGui::SameLine();
	ImGui::InputFloat2("", &sprite.offsetX);

	ImGui::Text("tile index: %d", sprite.index);
	ImGui::Text("tileset id: %d", sprite.tileset);
}

template<>
void componentImGui(Tile::ActorSpriteAnims& anims) {
	if (ImGui::TreeNode("walk", "walking")) {
		for (int i = 1; i < Tile::Direction::DirectionSize; i++) {
			ImGui::PushID(i);
			if (ImGui::TreeNode("", "TileMapDirection %d", i)) {
				ImGui::BeginTable("", anims.walk[i].size(), ImGuiTableFlags_SizingFixedFit);
				ImGui::TableNextRow();
				for (int j = 0; j < anims.walk[i].size(); j++) {
					ImGui::TableSetColumnIndex(j);
					ImGui::Text("%d", anims.walk[i][j]);
				}
				ImGui::EndTable();
				ImGui::TreePop();
			}
			ImGui::PopID();
		}
		ImGui::TreePop();
	}

	if (ImGui::TreeNode("sprint", "sprinting")) {
		for (int i = 1; i < Tile::Direction::DirectionSize; i++) {
			ImGui::PushID(i);
			if (ImGui::TreeNode("", "TileMapDirection %d", i)) {
				ImGui::BeginTable("", anims.sprint[i].size(), ImGuiTableFlags_SizingFixedFit);
				ImGui::TableNextRow();
				for (int j = 0; j < anims.sprint[i].size(); j++) {
					ImGui::TableSetColumnIndex(j);
					ImGui::Text("%d", anims.sprint[i][j]);
				}
				ImGui::EndTable();
				ImGui::TreePop();
			}
			ImGui::PopID();
		}
		ImGui::TreePop();
	}

	ImGui::Text("duration: %1.fms", anims.duration * 1000);
	ImGui::Text("index: %d", anims.index);
	ImGui::Text("timer: %f", anims.timer);
}

template<>
void componentImGui(Tile::Player& player) {
	if (player.speakingTo == ECS::NONE) {
		ImGui::Text("speakingTo: None");
	}
	else {
		ImGui::Text("speakingTo: %d", player.speakingTo);
	}
}

template<>
void componentImGui(Tile::NPC& npc) {
	ImGui::Text("Entity is an NPC");
}

template<>
void componentImGui(Tile::MapInteraction& action) {
	ImGui::Text(Tile::MapCommandNames[action.command.data.type]);
}

template<>
void componentImGui(Tile::MapCollisionInteraction& collisionAction) {
	ImGui::Text(Tile::MapCommandNames[collisionAction.command.data.type]);
	ImGui::Text("%d", collisionAction.mode);
}

template<>
void componentImGui(Tile::MapCommand& command) {
	ImGui::Text(Tile::MapCommandNames[command.data.type]);
}

template<>
void componentImGui(Tile::MapCommandList& list) {
	for (int i = 0; i < list.commands.size(); i++) {
		ImGui::Text("%d: %s", i, Tile::MapCommandNames[list.commands.at(i).data.type]);
	}
}
