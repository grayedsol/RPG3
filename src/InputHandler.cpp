/**
 * @file InputHandler.cpp
 * @author Grayedsol (grayedsol@gmail.com)
 * @copyright Copyright (c) 2024
 */
#include "InputHandler.hpp"
#include "GRY_Log.hpp"
#include "GRY_JSON.hpp"
#include "imgui_impl_sdl3.h"

/**
 * @brief File path to data for control scheme.
 * 
 */
static const char* CONTROLS_PATH = "assets/controls.json";

/**
 * @details
 * The default controls are mapped here.
 */
InputHandler::InputHandler() : keyboardState(SDL_GetKeyboardState(NULL)) {
	for (int i = 0; i < VirtualButton::VIRTUAL_BUTTON_SIZE; i++) {
		buttonState[i][0] = nullptr;
		buttonState[i][1] = nullptr;
	}
	for (int i = 0; i < GRY_MOUSECODE_COUNT; i++) { mouseState[i] = 0; } /* TODO change to GRY_RELEASED or something */

	resetControls();

	loadControls();
}

/**
 * @details
 * Logs an error and does not update the mapping under any of the following circumstances:
 * `code` evaluates to 0 (e.g., SDL_SCANCODE_UNKNOWN or GRY_MOUSECODE_UNKNOWN)
 * `button` evaluates to 0 (VirtualButton::GAME_NONE)
 * `code` already has a mapping to a VirtualButton
 * The primary input of `buttonState` is already bound and `secondary` is false
 * The secondary input of `buttonState` is already bound and `secondary` is true
 */
void InputHandler::mapInput(unsigned int code, VirtualButton button, bool mouse, bool secondary) {
	if (!code || !button) {
		GRY_Log("[InputHandler] Cannot map invalid code or VirtualButton::GAME_NONE.\n");
		return;
	}
	int index = secondary ? 1 : 0;
	if (mouse) {
		if (mouseButtons[code]) {
			GRY_Log("[InputHandler] Cannot map mouse input that has already been mapped.\n");
			return;
		}
		if (buttonState[button][index] != nullptr) {
			GRY_Log("[InputHandler] %s ", secondary ? "Secondary" : "Primary");
			GRY_Log("binding for VirtualButton already mapped.\n");
			return;
		}
		mouseButtons[code] = button;
		buttonState[button][index] = &mouseState[code];
	}
	else {
		if (keyButtons[code]) {
			GRY_Log("[InputHandler] Cannot map keyboard input that has already been mapped.\n");
			return;
		}
		if (buttonState[button][index] != nullptr) {
			GRY_Log("[InputHandler] %s ", secondary ? "Secondary" : "Primary");
			GRY_Log("binding for VirtualButton already mapped.\n");
			return;
		}
		keyButtons[code] = button;
		buttonState[button][index] = &keyboardState[code];
	}
}

void InputHandler::resetControls() {
	for (int i = 0; i < SDL_SCANCODE_COUNT; i++) { keyButtons[i] = VirtualButton::GAME_NONE; }
	for (int i = 0; i < GRY_MOUSECODE_COUNT; i++) { mouseButtons[i] = VirtualButton::GAME_NONE; }
}

void InputHandler::loadControls() {
	GRY_JSON::Document doc;
	GRY_JSON::loadDoc(doc, CONTROLS_PATH);

	for (int i = 1; i < VirtualButton::VIRTUAL_BUTTON_SIZE; i++) {
		if (!doc.HasMember(VirtualButtonStrings[i])) { continue; }

		auto& button = doc[VirtualButtonStrings[i]];

		if (!button.HasMember("Primary")) {
			GRY_Log("[InputHandler] Button %s has no Primary binding.\n", VirtualButtonStrings[i]);
			continue;
		}
		
		auto& primary = button["Primary"];
		mapInput(
			primary["code"].GetUint(),
			static_cast<VirtualButton>(i),
			primary.HasMember("mouse") ? primary["mouse"].GetBool() : false
		);

		if (!button.HasMember("Secondary")) { continue; }

		auto& secondary = button["Secondary"];
		mapInput(
			secondary["code"].GetUint(),
			static_cast<VirtualButton>(i),
			secondary.HasMember("mouse") ? secondary["mouse"].GetBool() : false,
			true
		);
	}
}

void InputHandler::process(bool& gameRunning) {
	/* Reset single key input for this frame */
	singleInput = GAME_NONE;

	/* Poll for events */
	SDL_Event event;
	while (SDL_PollEvent(&event)) {
		ImGui_ImplSDL3_ProcessEvent(&event);
		switch (event.window.type) {
		case SDL_EVENT_WINDOW_RESIZED:
			SDL_Log("Resized window.");
			break;
		default:
			break;
		}
		switch (event.type) {
		case SDL_EVENT_QUIT:
			gameRunning = false;
			break;
		case SDL_EVENT_MOUSE_BUTTON_DOWN:
			inputs.push_back(mouseButtons[event.button.button]); /**< Add active input to `inputs` */
			singleInput = mouseButtons[event.button.button]; /**< Set as the latest current frame input */
			/* No break here because we update the mouse state for both up and down mouse events */
		case SDL_EVENT_MOUSE_BUTTON_UP:
			mouseState[event.button.button] = event.button.down; /**< Update mouse state */
			break;
		case SDL_EVENT_MOUSE_MOTION:
			break;
		case SDL_EVENT_KEY_DOWN:
			if (event.key.repeat) { break; } /** Ensure it's not a key repeat */
			inputs.push_back(keyButtons[event.key.scancode]); /**< Add active input to `inputs` */
			singleInput = keyButtons[event.key.scancode]; /**< Set as the latest current frame input */
			break;
		case SDL_EVENT_KEY_UP:
			break;
		default:
			break;
		}
	}

	/* Remove from `inputs` until an active input is found */
	while (!inputs.empty() && !isPressing(inputs.back())) {
		inputs.pop_back();
	}
}
