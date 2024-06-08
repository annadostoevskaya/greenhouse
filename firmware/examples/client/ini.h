/*
 * Author: @github.com/annadostoevskaya
 * Filename: ini.h
 * Created: 18 May 2024 3:44:04 AM
 * Last Update: 28 May 2024 7:56:46 AM
 *
 * Description: <EMPTY>
 */

#pragma once

#include "NoCString.h"
#include <stdint.h>

int32_t ini_search_section(const char *src, const char *section);
int32_t ini_search_key(const char *src, const char *key);
NoCString ini_get_value(const char *src, const char *section, const char *key);
