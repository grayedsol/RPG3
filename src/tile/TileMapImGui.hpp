#pragma once
#include "GRY_ECSImGui.hpp"
#include "ComponentsImGui.hpp"
#include "TileComponentsImGui.hpp"
#include "TileMapECS.hpp"

static const char* TileMapECSComponentStrings[std::tuple_size_v<TileMapECS::TupleType>] = {
	"Position2",
	"Velocity2",
	"Hitbox",
	"Actor",
	"ActorSprite",
	"ActorSpriteAnims",
	"Player",
};

template<>
void imguiECS(TileMapECS& ecs) {
	ImGui::Begin("TileMapECS");
	imguiRecurseComponents(ecs.components, TileMapECSComponentStrings);
	ImGui::End();
}

void tileMapImGui(TileMapECS& ecs) {
	imguiECS(ecs);
}