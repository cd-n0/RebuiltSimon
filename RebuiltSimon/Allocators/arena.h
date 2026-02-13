#ifndef ARENA_H_
#define ARENA_H_

/* From cd-n0/cdstd/allocators with prefixes stripped and unix part of ARENA_TYPE_OS_RESERVE_COMMIT removed */

#include <stddef.h>

#ifdef __cplusplus
extern "C" {
#endif

	/* Opaque arena type */
	typedef struct arena_allocator_s arena_allocator_t;

	arena_allocator_t* arena_create(size_t size);
	void arena_destroy(arena_allocator_t* arena);
	void* arena_alloc(arena_allocator_t* arena, size_t size);
	void* arena_alloc_aligned(arena_allocator_t* arena, size_t size, unsigned int alignment);
	void arena_reset(arena_allocator_t* arena);
	char* arena_strdup(arena_allocator_t* arena, const char* s);
	char* arena_strndup(arena_allocator_t* arena, const char* s, size_t n);
	void* arena_memdup(arena_allocator_t* arena, const void* data, size_t size);
	void* arena_read_file(arena_allocator_t* arena, const char* path, size_t* length);
	size_t arena_get_usage(arena_allocator_t* arena);
	size_t arena_get_capacity(arena_allocator_t* arena);

#ifdef __cplusplus
}
#endif

#endif /* ARENA_H_ */
