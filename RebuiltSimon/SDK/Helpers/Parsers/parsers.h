#ifndef PARSERS_H_
#define PARSERS_H_

#include "RebuiltSimon/pch.h"
#include "RebuiltSimon/globals.h"

bool parse_rgba_u8(const char* s, uint8_t out[4], const char *default_value);
bool parse_rgb_u8(const char* s, uint8_t out[3], const char *default_value);
bool parse_normalized_pos(const char* s, int out[2], const char *default_value);

#endif /* PARSERS_H_ */