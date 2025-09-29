#include "RebuiltSimon/pch.h"
#include "RebuiltSimon/globals.h"

bool parse_rgba_u8(const char* s, uint8_t out[4], const char *default_value) {
    if (s && sscanf(s, "%hhd %hhd %hhd %hhd", &out[0], &out[1], &out[2], &out[3]) == 4) {
            return true;
    }
    else if (default_value && sscanf(default_value, "%hhd %hhd %hhd %hhd", &out[0], &out[1], &out[2], &out[3]) == 4) {
    }
    else {
        memset(out, 0, sizeof(out[0]) * 4);
    }
    return false;
}

bool parse_rgb_u8(const char* s, uint8_t out[3], const char* default_value) {
    if (s && sscanf(s, "%hhd %hhd %hhd", &out[0], &out[1], &out[2]) == 3) {
        return true;
    }
    else if (default_value && sscanf(default_value, "%hhd %hhd %hhd", &out[0], &out[1], &out[2]) == 3) {
    }
    else {
        memset(out, 0, sizeof(out[0]) * 3);
    }
    return false;
}

bool parse_normalized_pos(const char* s, int out[2], const char* default_value) {
    float x, y;
    bool result;
    if (s && sscanf(s, "%f %f", &x, &y) == 2) {
        result = true;
    }
    else if (default_value && sscanf(default_value, "%f %f", &x, &y) == 2) {
        result = false;
    }
    else {
        memset(out, 0, sizeof(out[0]) * 2);
        return false;
    }
    out[0] = (int)(x * screen_info.iWidth + .999f);
    out[1] = (int)(y * screen_info.iHeight + .999f);
    return result;
}