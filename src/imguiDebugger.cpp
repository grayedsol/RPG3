#include "imguiDebugger.hpp"
#include "GRY_Game.hpp"
#include "imgui.h"
#include "imgui_impl_sdl3.h"
#include "imgui_impl_sdlrenderer3.h"
#if defined(IMGUI_IMPL_OPENGL_ES2)
#include <SDL3/SDL_opengles2.h>
#else
#include <SDL3/SDL_opengl.h>
#endif

imguiDebugger::imguiDebugger(GRY_Game* game) : game(game) {}

imguiDebugger::~imguiDebugger() {
    ImGui_ImplSDLRenderer3_Shutdown();
    ImGui_ImplSDL3_Shutdown();
    ImGui::DestroyContext();
}

void imguiDebugger::init() {
	/* Setup context */
    IMGUI_CHECKVERSION();
    ImGui::CreateContext();
    io = &ImGui::GetIO();
	/* Enable keyboard and gamepad controls */
    io->ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;
    io->ConfigFlags |= ImGuiConfigFlags_NavEnableGamepad;

    /* Use dark style */
    ImGui::StyleColorsDark();

    /* Setup with SDL */
    ImGui_ImplSDL3_InitForSDLRenderer(game->getSDL().getWindow(), game->getSDL().getRenderer());
    ImGui_ImplSDLRenderer3_Init(game->getSDL().getRenderer());
}

void imguiDebugger::startFrame() {
	ImGui_ImplSDLRenderer3_NewFrame();
	ImGui_ImplSDL3_NewFrame();
	ImGui::NewFrame();
}

void imguiDebugger::process() {
	ImGui::Begin("Hello, world!");
	ImGui::Text("Application average %.3f ms/frame (%.1f FPS)", 1000.0f / io->Framerate, io->Framerate);
	ImGui::End();
}

void imguiDebugger::render(SDL_Renderer* renderer) {
	ImGui::Render();
	ImGui_ImplSDLRenderer3_RenderDrawData(ImGui::GetDrawData(), renderer);
}
