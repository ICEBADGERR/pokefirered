#ifdef PLATFORM_PC

#include "global.h"
#include "dma3.h"
#include <string.h>

void ClearDma3Requests(void) {}
void ProcessDma3Requests(void) {}

s16 RequestDma3Copy(const void *src, void *dest, u16 size, u8 mode)
{
    memcpy(dest, src, size);
    return 0;
}

s16 RequestDma3Fill(s32 value, void *dest, u16 size, u8 mode)
{
    memset(dest, value, size);
    return 0;
}

s16 WaitDma3Request(s16 index)
{
    return 0;
}

#endif // PLATFORM_PC
