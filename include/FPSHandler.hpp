/**
 * @file FPSHandler.hpp
 * @author Grayedsol (grayedsol@gmail.com)
 * @brief @copybrief FPSHandler
 * @copyright Copyright (c) 2024
 */
#pragma once
#include <stdint.h>

/**
 * @brief Calculates and manages the game speed.
 * 
 */
class FPSHandler {
private:
	typedef uint64_t Uint64;

	/**
	 * @brief Desired frames per second for the program.
	 * 
	 */
	const unsigned int TARGET_FPS;

	/**
	 * @brief Desired length of a frame in milliseconds.
	 * 
	 */
	const unsigned int frameLength;

	/**
	 * @brief Delta time for game processing, in seconds.
	 * 
	 * @details
	 * Initialized to `1.0 / TARGET_FPS`.
	 */
	double deltaTime;

	/**
	 * @brief Minimum delta time.
	 * 
	 * @details
	 * The game will run with at most `1.0 / minDelta` fps.
	 */
	double minDelta;

	/**
	 * @brief Maximum delta time.
	 * 
	 * @details
	 * The game will process in regards to this speed if the performance
	 * drops below `1.0 / maxDelta` fps.
	 */
	const double maxDelta = 0.05;

	/**
	 * @brief Start time of the current frame using SDL's counter.
	 * 
	 * @details
	 * Retrieved using `SDL_GetPerformanceCounter()`.
	 */
	Uint64 start = 0;

	/**
	 * @brief Start time of the current frame in milliseconds.
	 * 
	 * @details
	 * Retrieved using `SDL_GetTicks()`.
	 */
	Uint64 msStart = 0;
public:
	/**
	 * @brief Constructor.
	 * 
	 * @param TARGET_FPS The desired frames per second for the program.
	 */
	FPSHandler(const unsigned int TARGET_FPS);

	/**
	 * @brief Mark the start time of the frame.
	 * 
	 */
	void frameStart();

	/**
	 * @brief Determine the time it took to process since the last call to `frameStart()`.
	 *
	 * @returns The delta time in seconds.
	 * 
	 * @sa getDelta
	 * @sa getFPS
	 */
	void computeDelta();

	/**
	 * @brief Stall the program relative to the current frame's process time and `frameLength`.
	 * 
	 */
	void delay();

	/**
	 * @brief Get the delta time.
	 * 
	 * @return Delta time in seconds.
	 * 
	 * @sa computeDelta
	 * @sa getFPS
	 */
	double getDelta() { return deltaTime; }

	/**
	 * @brief Get the current FPS the game is running at.
	 * 
	 * @return Current frames per second.
	 * 
	 * @sa getDelta
	 */
	double getFPS() { return 1.0 / deltaTime; }
};
