#pragma once

struct GRY_Texture;
struct SDL_Renderer;

namespace Battle {
	class BattleScene;

	class BattleHUD {
	private:
		BattleScene* scene;

		SDL_Renderer* renderer = nullptr;

		const float* pixelScaling = nullptr;

		const GRY_Texture* hudTexture = nullptr;

		float renderX;
		float renderY;
		float renderW;
		float renderH;
	public:
		BattleHUD(BattleScene* scene);

		void init();

		void render();
	};
}