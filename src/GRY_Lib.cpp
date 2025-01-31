/**
 * @file GRY_Lib.cpp
 * @author Grayedsol (grayedsol@gmail.com)
 * @copyright Copyright (c) 2025
 */
#include "GRY_Lib.hpp"
#include <cstring>

char* GRY_copyString(const char *string) {
    return strcpy(new char[strlen(string) + 1], string);
}