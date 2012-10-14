#ifndef _CACHE_H_
#define _CACHE_H_

#include <stdint.h>

extern void flushDataCacheRegion(void *adr, uint32_t length);
extern void flushInstructionCacheRegion(void *adr, uint32_t length);

#endif /* _CACHE_H_ */
