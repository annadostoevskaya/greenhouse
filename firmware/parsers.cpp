/*
 * Author: @github.com/annadostoevskaya
 * Filename: parsers.cpp
 * Created: 13 Jun 2024 11:16:21 PM
 * Last Update: 13 Jun 2024 11:19:06 PM
 *
 * Description: <EMPTY>
 */

#include <ctype.h>
#include <stdint.h>
#include <string.h>

#include "parsers.h"

bool parse_mac(NoCString &s, uint8_t mac[6]) {
  if (s.end - s.begin != 17)
    return false;

  memset(mac, 0, 6);

  int8_t i = 0;
  for (const char *iter = s.begin; iter < s.end; iter += 1) {
    if (*iter == ':' || *iter == '-') {
      i += 1;
      if (i >= 6)
        break;
      continue;
    }

    mac[i] *= 16;
    if (isdigit(*iter)) {
      mac[i] += (*iter - '0');
      continue;
    }

    if (isxdigit(*iter)) {
      mac[i] += (toupper(*iter) - 55);
      continue;
    }

    memset(mac, 0, 6);
    return false;
  }

  return true;
}

bool parse_ip(NoCString &s, uint8_t ip[4]) {
  if (s.end - s.begin >= 16)
    return false;
  memset(ip, 0, 4);

  int8_t i = 0;
  for (const char *iter = s.begin; iter < s.end; iter += 1) {
    if (*iter == '.') {
      i += 1;
      if (i >= 4)
        break;
      continue;
    }

    ip[i] *= 10;
    if (isdigit(*iter)) {
      ip[i] += (*iter - '0');
      continue;
    }

    memset(ip, 0, 4);
    return false;
  }

  return true;
}
