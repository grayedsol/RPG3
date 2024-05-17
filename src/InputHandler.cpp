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
	resetControls();

	/**
	 * Hardcoded default controls.
	 * TODO: Change this.
	*/
	mapButton(SDL_SCANCODE_E, GAME_A);
	mapButton(SDL_SCANCODE_W, GAME_UP);
	mapButton(SDL_SCANCODE_S, GAME_DOWN);
	mapButton(SDL_SCANCODE_A, GAME_LEFT);
	mapButton(SDL_SCANCODE_D, GAME_RIGHT);
	mapButton(SDL_SCANCODE_Q, GAME_B);
	mapButton(SDL_SCANCODE_TAB, GAME_X);
	mapButton(SDL_SCANCODE_R, GAME_Y);
	mapButton(SDL_SCANCODE_RETURN, GAME_START);
	mapButton(SDL_SCANCODE_BACKSLASH, GAME_SELECT);
}

/**
 * @details
 * Logs an error and does not update the mapping if `scancode` is SDL_SCANCODE_UNKNOWN or 
 * `button` is VirtualButton::GAME_NONE.
 */
void InputHandler::mapButton(SDL_Scancode scancode, VirtualButton button) {
	if (scancode == SDL_SCANCODE_UNKNOWN || button == VirtualButton::GAME_NONE) {
		GRY_Log("[InputHandler] Cannot map SDL_SCANCODE_UNKOWN or VirtualButton::GAME_NONE.\n");
		return;
	}
	buttons[scancode] = button;
	scancodes[button] = scancode;
}

void InputHandler::resetControls() {
	for (int i = 0; i < SDL_NUM_SCANCODES; i++) { buttons[i] = VirtualButton::GAME_NONE; }
	for (int i = 0; i < VirtualButton::VIRTUAL_BUTTON_SIZE; i++) { scancodes[i] = SDL_SCANCODE_UNKNOWN; }
}

void InputHandler::processInput(SDL_Event& e) {
	if (e.key.type == SDL_EVENT_KEY_DOWN && e.key.repeat == 0) {
		/* Add the active input to `keyInputs` */
		keyInputs.push_back(e.key.keysym.scancode);
		singleKeyInput = e.key.keysym.scancode;
	}
	else if (e.key.type == SDL_EVENT_KEY_UP) {
		/* Remove inputs from `keyInputs` until an active input is found */
		while (keyInputs.size() > 0 && keyboardState[keyInputs.back()] == 0) {
			keyInputs.pop_back();
		}
	}
}

void InputHandler::process(bool& gameRunning) {
	/* Reset single key input for this frame */
	singleKeyInput = SDL_SCANCODE_UNKNOWN;

	SDL_Event event;
	/* Poll for events */
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
		case SDL_EVENT_MOUSE_MOTION:
			break;
		case SDL_EVENT_KEY_UP:
		case SDL_EVENT_KEY_DOWN:
			processInput(event);
			break;
		default:
			break;
		}
	}
}