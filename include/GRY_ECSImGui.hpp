/**
 * @file GRY_ECSImGui.hpp
 * @author Grayedsol (grayedsol@gmail.com)
 * @brief Utility functions for displaying ECS data with Dear ImGui.
 * @copyright Copyright (c) 2025
 */
#pragma once
#include "GRY_ECS.hpp"
#include "Components.hpp"
#include "imgui.h"

template<typename T>
void imguiComponentSet(ComponentSet<T>& set, std::size_t I, const char** componentStrings);

template<std::size_t I = 0, class... Ts>
typename std::enable_if<(I == std::tuple_size<std::tuple<ComponentSet<Ts>...>>::value)>::type
imguiRecurseComponents(std::tuple<ComponentSet<Ts>...>& components, const char** componentStrings = nullptr) {
}

template<std::size_t I = 0, class... Ts>
typename std::enable_if<(I < std::tuple_size<std::tuple<ComponentSet<Ts>...>>::value)>::type
imguiRecurseComponents(std::tuple<ComponentSet<Ts>...>& components, const char** componentStrings = nullptr) {
	imguiComponentSet(std::get<I>(components), I, componentStrings);
	imguiRecurseComponents<I+1,Ts...>(components, componentStrings);
}

template<class T>
void componentImGui(T& t) {
	ImGui::Text("No ImGui implementation for this component.");
}

template<class... Ts>
void imguiECS(GRY_ECS<Ts...>& ecs) {
	ImGui::Begin("ECS");
	imguiRecurseComponents(ecs.components);
	ImGui::End();
}

template<typename T>
void imguiComponentSet(ComponentSet<T>& set, std::size_t I, const char** componentStrings) {
	ImGui::PushID(I);
	const char* name = componentStrings ? componentStrings[I] : "ComponentSet %d";
	if (ImGui::TreeNode("", name, I)) {
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
