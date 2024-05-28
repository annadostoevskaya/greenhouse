#include <stddef.h>

class NoCString {
public:
  const char *begin;
  const char *end;

  NoCString(const char *begin_, const char *end_) : begin(begin_), end(end_) {}
  NoCString() : begin(nullptr), end(nullptr) {}

#if defined(_DEBUG)
  void print() {
    for (const char *iter = begin; iter < end; iter += 1) {
      Serial.print(*iter);
    }
  }

  void println() {
    print();
    Serial.println();
  }
#endif
};

size_t str_compare(NoCString *str, const char *cstr) {
  for (const char *iter = str->begin; iter < str->end; iter += 1) {
    if (*cstr == '\0' || *iter != *cstr++) {
      return false;
    }
  }

  return true;
}
