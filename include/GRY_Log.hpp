/**
 * @file GRY_Log.hpp
 * @author Grayedsol (grayedsol@gmail.com)
 * @brief Provides functions to output info to the console.
 * @copyright Copyright (c) 2024
 */
#pragma once
#include <cstdio>

#ifdef _WIN32
#include "windows.h"
/**
 * @brief Log a message to the console.
 * 
 * @param fmt A C-style formatted string.
 * @param ... Parameters to match % formatting tokens from `fmt`.
 */
#define GRY_Log(fmt, ...) {char cad[512]; snprintf(cad, 512, fmt, __VA_ARGS__);  OutputDebugStringA(cad);}
#endif

#ifndef _WIN32
/**
 * @brief Log a message to the console.
 * 
 * @param fmt A C-style formatted string.
 * @param ... Parameters to match % formatting tokens from `fmt`.
 * 
 */
#define GRY_Log(fmt, ...) printf(fmt, __VA_ARGS__)
#endif