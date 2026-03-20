#ifndef GUARD_MALLOC_H
#define GUARD_MALLOC_H

#include "global.h"

#define HEAP_SIZE 0x1C000
#ifndef PLATFORM_PC
#define malloc Alloc
#endif
#ifndef PLATFORM_PC
#define calloc(ct, sz) AllocZeroed((ct) * (sz))
#endif
#ifndef PLATFORM_PC
#define free Free
#endif

#define FREE_AND_SET_NULL(ptr)          \
{                                       \
    free(ptr);                          \
    ptr = NULL;                         \
}

#define TRY_FREE_AND_SET_NULL(ptr) if (ptr != NULL) FREE_AND_SET_NULL(ptr)

extern u8 gHeap[];
void *Alloc(u32 size);
void *AllocZeroed(u32 size);
void Free(void *pointer);
void InitHeap(void *pointer, u32 size);

#endif // GUARD_MALLOC_H
