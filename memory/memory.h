#ifndef MEMORY_H_
#define MEMORY_H_

#include <stdint.h>
#ifdef __cplusplus
extern "C" {
#endif

	void* cdmem_find_signature(const char* module, const char* signature);

#ifdef __cplusplus
}
#endif

#endif /* MEMORY_H_ */