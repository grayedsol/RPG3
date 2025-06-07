/**
 * @file TileMapImGui.hpp
 * @author Grayedsol (grayedsol@gmail.com)
 * @brief Utilities for using Dear ImGui in a Tile::MapScene.
 * @copyright Copyright (c) 2025
 */
#pragma once
#include "GRY_ECSImGui.hpp"
#include "ComponentsImGui.hpp"
#include "TileComponentsImGui.hpp"
#include "TileMapECS.hpp"

static const char* TileMapECSComponentStrings[std::tuple_size_v<Tile::MapECS::TupleType>] = {
	"Position2",
	"Velocity2",
	"Hitbox",
	"Actor",
	"ActorSprite",
	"ActorSpriteAnims",
	"Player",
	"NPC",
	"TileMapAction",
	"MapCommandList"
};

template<>
void imguiECS(Tile::MapECS& ecs) {
	ImGui::Begin("TileMapECS");
	imguiRecurseComponents(ecs.components, TileMapECSComponentStrings);
	ImGui::End();
}

inline void tileMapImGui(Tile::MapECS& ecs) {
	imguiECS(ecs);
}