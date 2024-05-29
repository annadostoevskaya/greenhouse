#include "NoCString.h"
#include <stddef.h>

NoCString::NoCString(const char *begin_, const char *end_)
    : begin(begin_), end(end_) {}

NoCString::NoCString() : NoCString(nullptr, nullptr) {}

#if defined(_DEBUG)
void NoCString::print() {
  for (const char *iter = begin; iter < end; iter += 1) {
    Serial.print(*iter);
  }
}

void NoCString::println() {
  print();
  Serial.println();
}
#endif

// TODO(annad): Move to operator=
size_t str_compare(NoCString *str, const char *cstr) {
  for (const char *iter = str->begin; iter < str->end; iter += 1) {
    if (*cstr == '\0' || *iter != *cstr++) {
      return false;
    }
  }

  return true;
}
