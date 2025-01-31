/**
 * @file GRY_Lib.hpp
 * @author Grayedsol (grayedsol@gmail.com)
 * @brief Miscellaneous functions.
 * @copyright Copyright (c) 2024
 */
#pragma once

/**
 * @brief Completely copies a string to a new string created using new[].
 * 
 * @param string String to copy.
 * 
 * @returns A pointer to the newly created string.
 */
char* GRY_copyString(const char *string);