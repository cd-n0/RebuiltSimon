/* TODO: Port the memory scanning module of cdstd to windows */
/* Credits to 8dcc: https://8dcc.github.io/programming/signature-scanning.html */
#define WIN32_LEAN_AND_MEAN
#include <windows.h>
#include <psapi.h>
#include "memory.h"

typedef struct module_s {
    void* module_base;
    void* module_end;
} module_t;

/* Convert a hex string like "FF" to a single byte (255) */
static uint8_t hex_to_byte(const char* hex) {
    int ret = 0;

    /* Skip leading spaces, if any */
    while (*hex == ' ')
        hex++;

    /* Store a byte (two digits of string) */
    for (int i = 0; i < 2 && hex[i] != '\0'; i++) {
        char c = hex[i];

        /* For example "E ", although the format should always be "0E" */
        if (c == ' ')
            break;

        uint8_t n = 0;
        if (c >= '0' && c <= '9')
            n = c - '0';
        else if (c >= 'a' && c <= 'f')
            n = 10 + c - 'a';
        else if (c >= 'A' && c <= 'F')
            n = 10 + c - 'A';

        /* Shift size of 0xF and add the next half of byte */
        ret <<= 4;
        ret |= n & 0xF;
    }

    return ret & 0xFF;
}

/* Search for `pattern' from `start' to `end'. */
static void* do_scan(void* start, void* end, const char* pattern) {
    if (!start || !end)
        return NULL;

    /* Skip preceding spaces from pattern, if any */
    while (*pattern == ' ')
        pattern++;

    /* Current position in memory and current position in pattern */
    uint8_t* mem_ptr = start;
    const uint8_t* pat_ptr = pattern;

    /* Iterate until we reach the end of the memory or the end of the pattern */
    while (mem_ptr < (uint8_t*)end && *pat_ptr != '\0') {
        /* Wildcard, always match */
        if (*pat_ptr == '?') {
            pat_ptr++;
            /* "A1 ?? ?? B2" -> "A1 ? ? B2" */
            if (*pat_ptr == '?')
                pat_ptr++;

            mem_ptr++;

            goto spaces;
        }

        /* Convert "E0" into 224.
         * TODO: Would be better to only do this once at the start of the
         * function with some kind of ida2bytes function (We would need a mask
         * for the '?' vs. 0x3F). */
        uint8_t cur_byte = hex_to_byte(pat_ptr);

        if (*mem_ptr == cur_byte) {
            /* Found exact byte match in sequence, go to next byte in memory */
            mem_ptr++;
            while (*pat_ptr != ' ' && *pat_ptr != '\0')
                pat_ptr++;
        }
        else {
            /* Byte didn't match, check pattern from the begining on the next
             * position in memory */
            start = (uint8_t*)start + 1;
            mem_ptr = start;
            pat_ptr = pattern;
        }

    spaces:
        /* Skip trailing spaces */
        while (*pat_ptr == ' ')
            pat_ptr++;
    }

    /* If we reached end of pattern, return the match. Otherwise, NULL */
    return (*pat_ptr == '\0') ? start : NULL;
}

static module_t get_module(const char* module) {
    HMODULE h_module = GetModuleHandleA(module);
    /* Return NULL if we can't find it */
    if (h_module == NULL) {
        return (module_t) { 0 };
    }
    MODULEINFO module_info;
    if (!GetModuleInformation(GetCurrentProcess(), h_module, &module_info, sizeof(MODULEINFO))) {
        return (module_t) { 0 };
    }

    return (module_t) { h_module, (char*)h_module + module_info.SizeOfImage };
}

void* cdmem_find_signature(const char* module, const char* signature) {
    module_t h_module = get_module(module);

    void* ptr = do_scan(h_module.module_base, h_module.module_end, signature);
    if (ptr != NULL) return ptr;

    return NULL;
}