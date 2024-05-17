/**
 * @file GRY_Lib.cpp
 * @author Grayedsol (grayedsol@gmail.com)
 * @copyright Copyright (c) 2024
 */
#include "GRY_Lib.hpp"

const char* GRY_copyString(const char *string) {
	return strcpy(new char[strlen(string) + 1], string);
}