/**
 * @file GRY_Game.cpp
 * @author Grayedsol (grayedsol@gmail.com)
 * @copyright Copyright (c) 2024
 */
#include "GRY_Game.hpp"
#include "GRY_Log.hpp"

GRY_Game::GRY_Game(int WINDOW_WIDTH, int WINDOW_HEIGHT, int TARGET_FPS, bool USE_VSYNC) :
	gsdl(WINDOW_WIDTH, WINDOW_HEIGHT, USE_VSYNC), fps(TARGET_FPS) {

	if (!gsdl.init()) { 
		GRY_Log("[Game] GRY_SDL initialization failed.\n");
		gameRunning = false;
		return;
	}
}

void GRY_Game::runGame() {
    auto& gameRenderer = gsdl.gameRenderer;

    /* Game loop */
	while (gameRunning) {
		/* Mark start time of frame */
		fps.frameStart();

		/* Clear renderer */
		SDL_RenderClear(gameRenderer);

		/* Handle inputs */
		input.process(gameRunning);

		/* Update game */
		process();
		scenes.process();

		fps.computeDelta();
		if (fps.isLagging()) { GRY_Log("[Game] Lagging.\n"); }

		/* Output */
		SDL_RenderPresent(gameRenderer);

		if (!gsdl.USE_VSYNC) { fps.delay(); }
	}
}
