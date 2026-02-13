#include "arena.h"
#include "arena_config.h"
#include <string.h>
#include <stddef.h>
#include <stdio.h>

/* Main arena implementations */
#include "arena_constant.c"
#include "arena_pointer_list.c"
#include "arena_reserve_commit.c"

/* Common functions */

inline void* arena_alloc(arena_allocator_t* arena, size_t size)
{
    return arena_alloc_aligned(arena, size, sizeof(void*));
}

char* arena_strdup(arena_allocator_t* arena, const char* str)
{
    if (!str) return NULL;
    size_t len = 0;
    const char* p = str;
    while (p && *p++) ++len;
    /* include null terminator */
    char* dst = arena_alloc(arena, len + 1);
    if (!dst) return NULL;
    ARENA_MEMCPY(dst, str, len);
    dst[len] = '\0';
    return dst;
}

char* arena_strndup(arena_allocator_t* arena, const char* str, size_t n)
{
    if (!str) return NULL;
    size_t len = 0;
    const char* p = str;
    while (len < n && p && *p++) ++len;
    char* dst = arena_alloc(arena, len + 1);
    if (!dst) return NULL;
    if (len > 0) ARENA_MEMCPY(dst, str, len);
    dst[len] = '\0';

    return dst;
}

/*
 * Note: Only use this if you know what you're doing. Making a copy of an
 * arbitrary data structure is not as straightforward as copying a string,
 * like the case of a structure that has pointers to other structures etc.
 *
 * This still has it's uses like duplicating binary blobs, images and more
 * but would lead to horrible bugs if used carelessly.
 */
void* arena_memdup(arena_allocator_t* arena, const void* data, size_t size)
{
    if (!data || size == 0) return NULL;
    void* dst = arena_alloc(arena, size);
    if (!dst) return NULL;
    ARENA_MEMCPY(dst, data, size);
    return dst;
}

/* Modified to use stdio instead of cdstd calls for file operations */
void* arena_read_file(arena_allocator_t* arena, const char* path, size_t* length) {
    FILE* f = fopen(path, "rb");
    if (!f) { perror("fopen"); return 0; }
    fseek(f, 0, SEEK_END);
    long size = ftell(f);
    fseek(f, 0, SEEK_SET);
    char* buf = arena_alloc(arena, size + 1);
    if (fread(buf, 1, size, f) != (size_t)size) {
        perror("fread");
        fclose(f);
        return 0;
    }
    buf[size] = '\0';
    fclose(f);
    if (length) *length = size;

    return buf;
}
