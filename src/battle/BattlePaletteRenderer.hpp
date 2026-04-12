#pragma once

struct GRY_Texture;
struct SDL_Renderer;

namespace Battle {
	class BattleScene;
	
	class PaletteRenderer {
	private:
		BattleScene* scene;

		SDL_Renderer* renderer = nullptr;

		const float* pixelScaling = nullptr;

		const GRY_Texture* paletteBoxTexture = nullptr;

		static const unsigned numDirections = 4;
		float renderX[numDirections] = { 0 };
		float renderY[numDirections] = { 0 };
		float renderW;
		float renderH;
	public:
		PaletteRenderer(BattleScene* scene);

		void init();

		void process();

		void setRenderPos(unsigned index, float x, float y);
	};
}
