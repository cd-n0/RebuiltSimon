#ifndef ARENA_CONFIG_H_
#define ARENA_CONFIG_H_

#include <stddef.h>

/* Select one of these defines at compile time to pick implementation:
   - ARENA_TYPE_CONSTANT_SIZE
   - ARENA_TYPE_POINTER_LIST
   - ARENA_TYPE_OS_RESERVE_COMMIT
*/
#if !defined(ARENA_TYPE_CONSTANT_SIZE) && !defined(ARENA_TYPE_POINTER_LIST) && !defined(ARENA_TYPE_OS_RESERVE_COMMIT)
#define ARENA_TYPE_POINTER_LIST
#endif



#ifndef ARENA_MALLOC
#include <stdlib.h>
#define ARENA_MALLOC malloc
#endif

#ifndef ARENA_FREE
#include <stdlib.h>
#define ARENA_FREE free
#endif

#ifndef ARENA_MEMCPY
#include <string.h>
#define ARENA_MEMCPY memcpy
#endif

#ifndef ARENA_MEMSET
#include <string.h>
#define ARENA_MEMSET memset
#endif

#define ARENA_ZERO_INITIALIZE

/* Default constant arena size, used for constant size arenas default
 * or the default of chunk size used by pointer list arenas */
#ifndef ARENA_DEFAULT_SIZE
#define ARENA_DEFAULT_SIZE (1024 * 1024) /* 1 MiB */
#endif

/* Default commit granularity, used for os specific memory commits */
#ifndef ARENA_COMMIT_GRANULARITY
#define ARENA_COMMIT_GRANULARITY 4096
#endif

#endif /* ARENA_CONFIG_H_ */
