#include "arena.h"
#include "arena_config.h"

#ifdef ARENA_TYPE_POINTER_LIST
#include <stdint.h>
#include <stdlib.h>
#include <string.h>

#ifndef ARENA_CHUNK_DEFAULT
#define ARENA_CHUNK_DEFAULT ARENA_DEFAULT_SIZE
#endif

typedef struct arena_chunk_header_s {
    struct arena_chunk_s* next;
    size_t size;
    size_t used;
} arena_chunk_header_t;

typedef struct arena_chunk_s {
    arena_chunk_header_t header;
    uint8_t data[1];
} arena_chunk_t;

struct arena_allocator_s {
    arena_chunk_t* head;
    size_t chunk_size;
};

static arena_chunk_t* arena_chunk_create(size_t chunk_size)
{
    arena_chunk_t* chunk = ARENA_MALLOC(sizeof(arena_chunk_header_t) + chunk_size);
    if (!chunk) return NULL;
    chunk->header.next = NULL;
    chunk->header.size = chunk_size;
    chunk->header.used = 0;
#ifdef ARENA_ZERO_INITIALIZE
    ARENA_MEMSET(chunk->data, 0, chunk_size);
#endif
    return chunk;
}

arena_allocator_t* arena_create(size_t size)
{
    arena_allocator_t* arena = ARENA_MALLOC(sizeof(arena_allocator_t));
    if (!arena) return NULL;
    arena->head = arena_chunk_create(size ? size : ARENA_CHUNK_DEFAULT);
    if (!arena->head) { ARENA_FREE(arena); return NULL; }
    arena->chunk_size = arena->head->header.size;
    return arena;
}

static size_t align_forward_ptr(size_t ptr, unsigned int alignment)
{
    size_t align = (alignment == 0) ? 1u : (size_t)alignment;
    size_t mask = align - 1;
    return (ptr + mask) & ~mask;
}

void* arena_alloc_aligned(arena_allocator_t* arena, size_t size, unsigned int alignment)
{
    arena_chunk_t* chunk = arena->head;
    if (!chunk) {
        chunk = arena_chunk_create(arena->chunk_size);
        if (!chunk) return NULL;
        arena->head = chunk;
    }

    size_t base = (size_t)(chunk->data);
    size_t current = base + chunk->header.used;
    size_t aligned = align_forward_ptr(current, alignment);
    size_t need = (aligned - base) + size;

    if (need > chunk->header.size) {
        /* allocate a new chunk, at least big enough for requested size */
        size_t chunk_sz = arena->chunk_size;
        if (size + sizeof(void*) + 16 > chunk_sz) chunk_sz = size + 16;
        arena_chunk_t* n = arena_chunk_create(chunk_sz);
        if (!n) return NULL;
        n->header.next = arena->head;
        arena->head = n;
        chunk = n;
        base = (size_t)(chunk->data);
        current = base + chunk->header.used;
        aligned = align_forward_ptr(current, alignment);
        need = (aligned - base) + size;
        if (need > chunk->header.size) return NULL; /* still cannot satisfy */
    }

    void* ptr = (void*)aligned;
    chunk->header.used = need;
#ifdef ARENA_ZERO_INITIALIZE
    ARENA_MEMSET(ptr, 0, size);
#endif
    return ptr;
}

void arena_reset(arena_allocator_t* arena)
{
    /* Free all chunks except the head and reset the head */
    arena_chunk_t* head = arena->head;
    if (!head) return;
    arena_chunk_t* chunk = head->header.next;
    while (chunk) {
        arena_chunk_t* next = chunk->header.next;
        ARENA_FREE(chunk);
        chunk = next;
    }
    head->header.next = NULL;
    head->header.used = 0;
}

void arena_destroy(arena_allocator_t* arena)
{
    if (!arena) return;
    arena_chunk_t* chunk = arena->head;
    while (chunk) {
        arena_chunk_t* next = chunk->header.next;
        ARENA_FREE(chunk);
        chunk = next;
    }
    ARENA_FREE(arena);
}

size_t arena_get_usage(arena_allocator_t* arena)
{
    if (!arena) return 0;
    size_t total = 0;
    for (arena_chunk_t* chunk = arena->head; chunk; chunk = chunk->header.next) {
        total += chunk->header.used;
    }
    return total;
}

size_t arena_get_capacity(arena_allocator_t* arena)
{
    if (!arena) return 0;
    size_t total = 0;
    for (arena_chunk_t* chunk = arena->head; chunk; chunk = chunk->header.next) {
        total += chunk->header.size;
    }
    return total;
}

#endif /* ARENA_TYPE_POINTER_LIST */
