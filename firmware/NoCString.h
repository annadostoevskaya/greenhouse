/*
 * Author: @github.com/annadostoevskaya
 * Filename: NoCString.h
 * Created: 18 May 2024 3:40:17 AM
 * Last Update: 13 Jun 2024 11:29:10 PM
 *
 * Description: <EMPTY>
 */

#pragma once

#include <Arduino.h>
#include <stddef.h>

class NoCString {
public:
  const char *begin;
  const char *end;

  NoCString(const char *begin_, const char *end_);
  NoCString();

  void print(Print &o);
  void println(Print &o);
  bool is_empty();
  bool cmp(const char *cstr);
};
