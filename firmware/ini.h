/*
 * Author: @github.com/annadostoevskaya
 * Filename: ini.h
 * Created: 18 May 2024 3:44:04 AM
 * Last Update: 18 May 2024 3:48:26 AM
 *
 * Description: <EMPTY>
 */

#pragma once

#include "NoCString.h"

size_t ini_search_section(const char *src, const char *section);
size_t ini_search_key(const char *src, const char *key);
NoCString ini_get_value(const char *src, const char *section, const char *key);
