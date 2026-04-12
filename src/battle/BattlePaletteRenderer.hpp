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

		static const unsigned numTexts = 5;
		static const unsigned numBoxes = 4;
		float renderX[numTexts] = { 0 };
		float renderY[numTexts] = { 0 };
		float renderW;
		float renderH;

		float textYOffset = 0;
	public:
		PaletteRenderer(BattleScene* scene);

		void init();

		void process();

		void setRenderPos(unsigned index, float x, float y);
	};
}
