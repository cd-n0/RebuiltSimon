#include "platform_reserve_commit.h"

#if defined(_WIN32) || defined(_WIN64)

#include <windows.h>

int platform_reserve_memory(void** out_addr, size_t size)
{
    void* ptr = VirtualAlloc(NULL, (SIZE_T)size, MEM_RESERVE, PAGE_NOACCESS);
    if (!ptr) return -1;
    *out_addr = ptr;
    return 0;
}

int platform_commit_memory(void* addr, size_t size)
{
    void* p = VirtualAlloc(addr, (SIZE_T)size, MEM_COMMIT, PAGE_READWRITE);
    return p ? 0 : -1;
}

int platform_decommit_memory(void* addr, size_t size)
{
    return VirtualFree(addr, (SIZE_T)size, MEM_DECOMMIT) ? 0 : -1;
}

int platform_release_memory(void* addr, size_t size)
{
    (void)size;
    return VirtualFree(addr, 0, MEM_RELEASE) ? 0 : -1;
}

#endif /* _WIN32 || _WIN64 */
