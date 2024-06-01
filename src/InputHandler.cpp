/**
 * @file InputHandler.cpp
 * @author Grayedsol (grayedsol@gmail.com)
 * @copyright Copyright (c) 2024
 */
#include "InputHandler.hpp"

/**
 * @details
 * The default controls are mapped here.
 */
InputHandler::InputHandler() : keyboardState(SDL_GetKeyboardState(NULL)) {
	for (int i = 0; i < VirtualButton::VIRTUAL_BUTTON_SIZE; i++) {
		VButtonState[i][0] = nullptr;
		VButtonState[i][1] = nullptr;
	}
	for (int i = 0; i < GRY_NUM_MOUSECODES; i++) { mouseState[i] = SDL_RELEASED; }

	resetControls();

	/**
	 * Hardcoded default controls.
	 * TODO: Change this.
	*/
	mapInput(SDL_SCANCODE_E, GAME_A, false);
	mapInput(SDL_SCANCODE_W, GAME_UP, false);
	mapInput(GRY_MOUSE_LEFT, GAME_UP, true, true);
	mapInput(SDL_SCANCODE_S, GAME_DOWN, false);
	mapInput(SDL_SCANCODE_A, GAME_LEFT, false);
	mapInput(SDL_SCANCODE_D, GAME_RIGHT, false);
	mapInput(SDL_SCANCODE_Q, GAME_B, false);
	mapInput(SDL_SCANCODE_TAB, GAME_X, false);
	mapInput(SDL_SCANCODE_R, GAME_Y, false);
	mapInput(SDL_SCANCODE_RETURN, GAME_START, false);
	mapInput(SDL_SCANCODE_BACKSLASH, GAME_SELECT, false);
}

/**
 * @details
 * Logs an error and does not update the mapping if `code` evaluates to 0 or if
 * `button` is VirtualButton::GAME_NONE.
 */
void InputHandler::mapInput(unsigned int code, VirtualButton button, bool mouse, bool secondary) {
	if (!code || !button) {
		GRY_Log("[InputHandler] Cannot map invalid code or VirtualButton::GAME_NONE.\n");
		return;
	}
	int index = secondary ? 1 : 0;
	/* TODO: check for if controls are being overwritten here */
	if (mouse) {
		if (mouseButtons[code]) {
			GRY_Log("[InputHandler] Cannot map mouse input that has already been mapped.\n");
			return;
		}
		if (VButtonState[button][index] != nullptr) {
			GRY_Log("[InputHandler] %s binding for VirtualButton already mapped\n.", secondary ? "Secondary" : "Primary");
			return;
		}
		mouseButtons[code] = button;
		VButtonState[button][index] = &mouseState[code];
	}
	else {
		if (keyButtons[code]) {
			GRY_Log("[InputHandler] Cannot map keyboard input that has already been mapped.\n");
			return;
		}
		if (VButtonState[button][index] != nullptr) {
			GRY_Log("[InputHandler] %s binding for VirtualButton already mapped\n.", secondary ? "Secondary" : "Primary");
			return;
		}
		keyButtons[code] = button;
		VButtonState[button][index] = &keyboardState[code];
	}
}

void InputHandler::resetControls() {
	for (int i = 0; i < SDL_NUM_SCANCODES; i++) { keyButtons[i] = VirtualButton::GAME_NONE; }
	for (int i = 0; i < GRY_NUM_MOUSECODES; i++) { mouseButtons[i] = VirtualButton::GAME_NONE; }
}

void InputHandler::processMouseInput(const SDL_Event &e) {
	if (e.button.type == SDL_EVENT_MOUSE_BUTTON_DOWN) {
		inputs.push_back(mouseButtons[e.button.button]);
		singleInput = mouseButtons[e.button.button];
	}
	mouseState[e.button.button] = e.button.state;
}

void InputHandler::processKeyInput(const SDL_Event& e) {
	if (!e.key.repeat) {
		/* Add the active input to `inputs` */
		inputs.push_back(keyButtons[e.key.keysym.scancode]);
		singleInput = keyButtons[e.key.keysym.scancode];
	}
}

void InputHandler::process(bool& gameRunning) {
	/* Reset single key input for this frame */
	singleInput = GAME_NONE;

	/* Poll for events */
	SDL_Event event;
	while (SDL_PollEvent(&event)) {
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
		case SDL_EVENT_MOUSE_BUTTON_UP:
		case SDL_EVENT_MOUSE_BUTTON_DOWN:
			processMouseInput(event);
			break;
		case SDL_EVENT_MOUSE_MOTION:
			break;
		case SDL_EVENT_KEY_UP:
			break;
		case SDL_EVENT_KEY_DOWN:
			processKeyInput(event);
			break;
		default:
			break;
		}
	}

	/* Remove from inputs until an active input is found */
	while (!inputs.empty() && !isPressing(inputs.back())) {
		inputs.pop_back();
	}
}

const bool InputHandler::isPressing(VirtualButton b) const {
	return b &&
		(VButtonState[b][0] && *VButtonState[b][0]) ||
		(VButtonState[b][1] && *VButtonState[b][1]);
}
