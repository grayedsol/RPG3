#pragma once
#include "GRY_ECSImGui.hpp"
#include "Components.hpp"

template<>
void componentImGui(Position2& pos) {
	ImGui::Text("position");
	ImGui::SameLine();
	ImGui::InputFloat2("", &pos.x);
}

template<>
void componentImGui(Velocity2& vel) {
	ImGui::Text("velocity");
	ImGui::SameLine();
	ImGui::InputFloat2("", &vel.x);
}

template<>
void componentImGui(Hitbox& hitbox) {
	ImGui::Text("x: %f", hitbox.x);
	ImGui::Text("y: %f", hitbox.y);
	ImGui::Text("w: %f", hitbox.w);
	ImGui::Text("h: %f", hitbox.h);
}
