/**
 * @file imguiDebugger.hpp
 * @author Grayedsol (grayedsol@gmail.com)
 * @brief @copybrief imguiDebugger
 * @copyright Copyright (c) 2024
 */
#pragma once

class GRY_Game;
struct ImGuiIO;
struct SDL_Renderer;

/**
 * @brief In-game debugger UI.
 * 
 */
class imguiDebugger {
private:

	GRY_Game* game;

	ImGuiIO* io;

public:
	imguiDebugger(GRY_Game* game);

	~imguiDebugger();

	void init();

	void startFrame();

	void process();

	void render(SDL_Renderer* renderer);
};