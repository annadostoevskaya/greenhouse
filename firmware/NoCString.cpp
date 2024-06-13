#include "NoCString.h"
#include <Arduino.h>
#include <stddef.h>

NoCString::NoCString(const char *begin_, const char *end_)
    : begin(begin_), end(end_) {}

NoCString::NoCString() : NoCString(nullptr, nullptr) {}

void NoCString::print(Print &o) {
  for (const char *iter = begin; iter < end; iter += 1) {
    o.print(*iter);
  }
}

void NoCString::println(Print &o) {
  print(o);
  o.println();
}

bool NoCString::is_empty() { return begin == end; }

bool NoCString::cmp(const char *cstr) {
  for (const char *iter = begin; iter < end; iter += 1) {
    if (*cstr == '\0' || *iter != *cstr++) {
      return false;
    }
  }

  return true;
}
