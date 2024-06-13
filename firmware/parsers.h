/*
 * Author: @github.com/annadostoevskaya
 * Filename: parsers.h
 * Created: 13 Jun 2024 11:15:22 PM
 * Last Update: 13 Jun 2024 11:17:45 PM
 *
 * Description: <EMPTY>
 */

#pragma once

#include <stdint.h>

#include "NoCString.h"

bool parse_mac(NoCString &s, uint8_t mac[6]);
bool parse_ip(NoCString &s, uint8_t ip[4]);
