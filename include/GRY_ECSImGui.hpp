#pragma once
#include "GRY_ECS.hpp"
#include "Components.hpp"
#include "../src/tile/TileComponents.hpp"
#include "imgui.h"

template<class T>
void componentImGui(T t);

template<typename T>
void imguiComponentSet(ComponentSet<T>& set, size_t I);

template<std::size_t I = 0, class... Ts>
typename std::enable_if<(I == std::tuple_size<std::tuple<ComponentSet<Ts>...>>::value)>::type
imguiECS(std::tuple<ComponentSet<Ts>...>& components) {
}

template<std::size_t I = 0, class... Ts>
typename std::enable_if<(I < std::tuple_size<std::tuple<ComponentSet<Ts>...>>::value)>::type
imguiECS(std::tuple<ComponentSet<Ts>...>& components) {
	imguiComponentSet(std::get<I>(components), I);
	imguiECS<I+1,Ts...>(components);
}

template<class... Ts>
void imgui_ecs(GRY_ECS<Ts...>& ecs) {
	ImGui::Begin("ECS");
	imguiECS(ecs.components);
	ImGui::End();
}

template<class T>
void componentImGui(T t) {}

template<>
void componentImGui(Actor actor) {
	if (ImGui::BeginTable("actor", 2)) {
		ImGui::TableSetupColumn("Name");
		ImGui::TableSetupColumn("Value");
		ImGui::TableHeadersRow();

		ImGui::TableNextRow();
		ImGui::TableNextColumn();
		ImGui::Text("speed");
		ImGui::TableNextColumn();
		ImGui::Text("%f", actor.speed);

		ImGui::TableNextColumn();
		ImGui::Text("direction");
		ImGui::TableNextColumn();
		ImGui::Text("%d", actor.direction);

		ImGui::TableNextColumn();
		ImGui::Text("moving");
		ImGui::TableNextColumn();
		ImGui::Text("%d", actor.moving);

		ImGui::TableNextColumn();
		ImGui::Text("sprinting");
		ImGui::TableNextColumn();
		ImGui::Text("%d", actor.sprinting);

		ImGui::EndTable();
	}
}

template<typename T>
void imguiComponentSet(ComponentSet<T>& set, size_t I) {
	ImGui::PushID(I);
	if (ImGui::TreeNode("", "Component Set %d", I)) {
		for (int i = 0; i < set.dense.size(); i++) {
			ECS::entity e = set.dense.at(i);
			ImGui::PushID(e);
			if (ImGui::TreeNode("", "Entity %d", e)) {
				componentImGui(set.value.at(i));
				ImGui::TreePop();
			}
			ImGui::PopID();
		}
		ImGui::TreePop();
	}
	ImGui::PopID();
}
