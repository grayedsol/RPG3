/**
 * @file GRY_Game.cpp
 * @author Grayedsol (grayedsol@gmail.com)
 * @copyright Copyright (c) 2024
 */
#include "GRY_Game.hpp"
#include "GRY_Log.hpp"
#include "imgui.h"
#include "imgui_impl_sdl3.h"
#include "imgui_impl_sdlrenderer3.h"
#if defined(IMGUI_IMPL_OPENGL_ES2)
#include <SDL3/SDL_opengles2.h>
#else
#include <SDL3/SDL_opengl.h>
#endif

GRY_Game::GRY_Game(int WINDOW_WIDTH, int WINDOW_HEIGHT, int TARGET_FPS, bool USE_VSYNC) :
	gsdl(WINDOW_WIDTH, WINDOW_HEIGHT, USE_VSYNC), fps(TARGET_FPS), imguiDebug(this) {

	if (!gsdl.init()) { 
		GRY_Log("[Game] GRY_SDL initialization failed.\n");
		gameRunning = false;
		return;
	}

	imguiDebug.init();
}

void GRY_Game::runGame() {
    auto& gameRenderer = gsdl.gameRenderer;
	ImGuiIO& io = ImGui::GetIO();

    /* Game loop */
	while (gameRunning) {
		/* Mark start time of frame */
		fps.frameStart();

		/* Clear renderer */
		SDL_RenderClear(gameRenderer);

		/* Handle inputs */
		input.process(gameRunning);
		imguiDebug.startFrame();

		/* Update game */
		imguiDebug.process();
		process();
		scenes.process();
		
		imguiDebug.render(gameRenderer);

		fps.computeDelta();
		if (fps.isLagging()) { GRY_Log("[Game] Lagging.\n"); }

		/* Output */
		SDL_RenderPresent(gameRenderer);

		if (!gsdl.USE_VSYNC) { fps.delay(); }
	}
}

/**
 * @details
 * Toggles the debug screen if SELECT is pressed while START is being pressed.
 */
void GRY_Game::process() {
}
