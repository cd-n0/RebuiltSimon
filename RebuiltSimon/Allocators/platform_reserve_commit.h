/* platform_reserve_commit.h */
#ifndef PLATFORM_RESERVE_COMMIT_H_
#define PLATFORM_RESERVE_COMMIT_H_

#include <stddef.h>

int platform_reserve_memory(void** out_addr, size_t size);
int platform_commit_memory(void* addr, size_t size);
int platform_decommit_memory(void* addr, size_t size);
int platform_release_memory(void* addr, size_t size);

#endif /* PLATFORM_RESERVE_COMMIT_H_ */
