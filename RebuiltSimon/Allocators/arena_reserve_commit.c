#include "arena.h"
#include "arena_config.h"

#ifdef ARENA_TYPE_OS_RESERVE_COMMIT

#include "platform_reserve_commit.h"
#include "win32_reserve_commit.c"
/* #include "unix_reserve_commit.c" */
#include <stdlib.h>
#include <string.h>

struct arena_allocator_s {
    uint8_t* base;
    size_t reserved_size;
    size_t committed_size;
    size_t offset;
};

arena_allocator_t* arena_create(size_t size)
{
    size_t rs = size ? size : ARENA_DEFAULT_SIZE;
    void* addr = NULL;
    if (platform_reserve_memory(&addr, rs) != 0) return NULL;
    arena_allocator_t* a = ARENA_MALLOC(sizeof(arena_allocator_t));
    if (!a) { platform_release_memory(addr, rs); return NULL; }
    a->base = addr;
    a->reserved_size = rs;
    a->committed_size = 0;
    a->offset = 0;
    return a;
}

static size_t arena_align_forward(size_t ptr, size_t alignment)
{
    size_t align = (alignment == 0) ? 1u : alignment;
    size_t mask = align - 1;
    return (ptr + mask) & ~mask;
}

void* arena_alloc_aligned(arena_allocator_t* arena, size_t size, unsigned int alignment)
{
    size_t base_ptr = (size_t)arena->base;
    size_t cur = base_ptr + arena->offset;
    size_t aligned = arena_align_forward(cur, alignment);
    size_t needed_offset = (aligned - base_ptr) + size;
    if (needed_offset > arena->reserved_size) return NULL;

    /* ensure committed */
    if (needed_offset > arena->committed_size) {
        size_t commit_to = arena->committed_size;
        if (commit_to == 0) commit_to = 1;
        while (commit_to < needed_offset) {
            commit_to += ARENA_COMMIT_GRANULARITY;
        }
        if (commit_to > arena->reserved_size) commit_to = arena->reserved_size;
        if (platform_commit_memory(arena->base + arena->committed_size, commit_to - arena->committed_size) != 0) {
            return NULL;
        }
        arena->committed_size = commit_to;
    }

    void* ptr = (void*)(aligned);
    arena->offset = needed_offset;
#ifdef ARENA_ZERO_INITIALIZE
        ARENA_MEMSET(ptr, 0, size);
#endif
    return ptr;
}

void arena_reset(arena_allocator_t* arena)
{
    /* optionally decommit pages; keep reservation */
    if (arena->committed_size > 0) {
        platform_decommit_memory(arena->base, arena->committed_size);
        arena->committed_size = 0;
    }
    arena->offset = 0;
}

void arena_destroy(arena_allocator_t* arena)
{
    if (!arena) return;
    if (arena->base) {
        platform_release_memory(arena->base, arena->reserved_size);
    }
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
    return arena->reserved_size;
}

#endif /* ARENA_TYPE_OS_RESERVE_COMMIT */
