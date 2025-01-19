/**
 * @file GRY_Lib.hpp
 * @author Grayedsol (grayedsol@gmail.com)
 * @brief Miscellaneous functions.
 * @copyright Copyright (c) 2024
 */
#pragma once
#include <cstring>

/**
 * @brief Completely copies a string to a new string created using new[].
 * 
 * @param string String to copy.
 * 
 * @returns A pointer to the newly created string.
 */
inline char* GRY_copyString(const char *string) {
    return strcpy(new char[strlen(string) + 1], string);
}