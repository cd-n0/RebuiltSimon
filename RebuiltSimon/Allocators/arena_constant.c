#include "arena.h"
#include "arena_config.h"

#ifdef ARENA_TYPE_CONSTANT_SIZE

#include <stdint.h>
#include <stdlib.h>
#include <string.h>

/* Arena layout for constant-size arena */
struct arena_allocator_s {
    uint8_t* base;
    size_t size;
    size_t offset;
};

arena_allocator_t* arena_create(size_t size)
{
    size_t alloc_size = size ? size : ARENA_DEFAULT_SIZE;
    void* mem = ARENA_MALLOC(alloc_size);
    if (!mem) return NULL;
    arena_allocator_t* a = (arena_allocator_t*)ARENA_MALLOC(sizeof(arena_allocator_t));
    if (!a) { ARENA_FREE(mem); return NULL; }
    a->base = (uint8_t*)mem;
    a->size = alloc_size;
    a->offset = 0;
    return a;
}

static size_t arena_align_forward(size_t ptr, unsigned int alignment)
{
    size_t align = (alignment == 0) ? 1u : (size_t)alignment;
    size_t mask = align - 1;
    return (ptr + mask) & ~mask;
}

void* arena_alloc_aligned(arena_allocator_t* arena, size_t size, unsigned int alignment)
{
    size_t base = (size_t)(arena->base);
    size_t cur = base + arena->offset;
    size_t aligned = arena_align_forward(cur, alignment);
    size_t new_offset = (aligned - base) + size;
    if (new_offset > arena->size) return NULL;
    void* ptr = (void*)(aligned);
    arena->offset = new_offset;
#ifdef ARENA_ZERO_INITIALIZE
    ARENA_MEMSET(ptr, 0, size);
#endif
    return ptr;
}

void arena_reset(arena_allocator_t* arena)
{
    arena->offset = 0;
}

void arena_destroy(arena_allocator_t* arena)
{
    if (!arena) return;
    if (arena->base) ARENA_FREE(arena->base);
    ARENA_FREE(arena);
}

size_t arena_get_usage(arena_allocator_t* arena)
{
    if (!arena) return 0;
    return arena->offset;
}

size_t arena_get_capacity(arena_allocator_t* arena)
{
    if (!arena) return 0;
    return arena->size;
}

#endif /* ARENA_TYPE_CONSTANT_SIZE */
