#ifdef PLATFORM_PC

#include "global.h"
#include "decompress.h"
#include <string.h>

// GBA LZ77 decompression - same algorithm as GBA BIOS LZ77UnCompWram
// Header: 4 bytes - byte 0 = 0x10 (type), bytes 1-3 = decompressed size
void LZ77UnCompWram(const void *src, void *dest)
{
    const u8 *in = (const u8 *)src;
    u8 *out = (u8 *)dest;

    // Read header
    u32 header = *(u32 *)in;
    in += 4;
    u32 decompSize = header >> 8;

    u32 written = 0;
    while (written < decompSize)
    {
        u8 flags = *in++;
        for (int i = 7; i >= 0 && written < decompSize; i--)
        {
            if (flags & (1 << i))
            {
                // Compressed block
                u8 b0 = *in++;
                u8 b1 = *in++;
                u32 length = (b0 >> 4) + 3;
                u32 disp = (((b0 & 0xF) << 8) | b1) + 1;
                for (u32 j = 0; j < length && written < decompSize; j++)
                {
                    out[written] = out[written - disp];
                    written++;
                }
            }
            else
            {
                // Uncompressed byte
                out[written++] = *in++;
            }
        }
    }
}

// LZ77UnCompVram is the same on PC since we don't have real VRAM
void LZ77UnCompVram(const void *src, void *dest)
{
    LZ77UnCompWram(src, dest);
}

// Decompression buffer used by various functions
u8 gDecompressionBuffer[0x4000] = {0};

void LZDecompressWram(const void *src, void *dest)
{
    LZ77UnCompWram(src, dest);
}

void LZDecompressVram(const void *src, void *dest)
{
    LZ77UnCompWram(src, dest);
}

#endif // PLATFORM_PC
