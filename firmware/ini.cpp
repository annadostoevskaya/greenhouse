#include "NoCString.h"
#include <stddef.h>
#include <stdint.h>

int32_t ini_search_section(const char *src, const char *section) {
  size_t cursor = 0;
  const char *begin = nullptr;
  const char *end = nullptr;

  while (src[cursor] != '\0') {
    while (src[cursor] != '\0' && src[cursor++] != '[')
      ;
    ;

    begin = src + cursor;

    while (src[cursor] != '\0' && src[cursor++] != ']')
      ;
    ;

    end = src + cursor - 1;

    while (src[cursor] != '\0' && src[cursor++] != '\n')
      ;
    ;

    NoCString s{begin, end};
    if (str_compare(&s, section))
      return cursor;
  }

  return -1;
}

int32_t ini_search_key(const char *src, const char *key) {
  size_t cursor = 0;
  const char *begin = nullptr;
  const char *end = nullptr;

  while (src[cursor] != '\0') {
    begin = src + cursor;

    while (src[cursor] != '\0' && src[cursor++] != '=')
      ;
    ;

    end = src + cursor - 1;
    while (*(end - 1) == ' ')
      end -= 1;

    NoCString k{begin, end};
    if (str_compare(&k, key)) {
      while (src[cursor] != '\0' && src[cursor] == ' ')
        cursor += 1;
      return cursor;
    }

    while (src[cursor] != '\0' && src[cursor++] != '\n')
      ;
    ;
  }

  return -1;
}

NoCString ini_get_value(const char *src, const char *section, const char *key) {
  int32_t section_shift = ini_search_section(src, section);
  if (section_shift == -1) {
    return {};
  }

  int32_t key_shift = ini_search_key(src + section_shift, key);
  if (key_shift == -1) {
    return {};
  }

  size_t shift = section_shift + key_shift;

  size_t cursor = shift;
  while (src[cursor] != '\0' && src[cursor] != '\r' && src[cursor++] != '\n')
    ;
  ;

  return {(src + shift), (src + cursor - 1)};
}
