#pragma once
#include "GRY_ECSImGui.hpp"
#include "Components.hpp"

template<>
void componentImGui(Position2& pos) {
	ImGui::Text("position");
	ImGui::SameLine();
	ImGui::InputFloat2("", pos.data);
}

template<>
void componentImGui(Velocity2& vel) {
	ImGui::Text("velocity");
	ImGui::SameLine();
	ImGui::InputFloat2("", vel.data);
}

template<>
void componentImGui(Hitbox& hitbox) {
	ImGui::Text("center: %f, %f", hitbox.centerX, hitbox.centerY);
	ImGui::Text("halfWidth: %f", hitbox.halfWidth);
	ImGui::Text("halfHeight: %f", hitbox.halfHeight);
}
