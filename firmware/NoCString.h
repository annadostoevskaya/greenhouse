/*
 * Author: @github.com/annadostoevskaya
 * Filename: NoCString.h
 * Created: 18 May 2024 3:40:17 AM
 * Last Update: 29 May 2024 8:19:52 AM
 *
 * Description: <EMPTY>
 */

#pragma once

#include <stddef.h>

class NoCString {
public:
  const char *begin;
  const char *end;

  NoCString(const char *begin_, const char *end_);
  NoCString();

#if defined(_DEBUG)
  void print();
  void println();
#endif
};

size_t str_compare(NoCString *str, const char *cstr);
