#ifdef PLATFORM_PC

#include "global.h"
#include "decompress.h"
#include "new_menu_helpers.h"

void ClearScheduledBgCopiesToVram(void) {}
void ScheduleBgCopyTilemapToVram(u8 bgId) {}
void DoScheduledBgTilemapCopiesToVram(void) {}
void ResetTempTileDataBuffers(void) {}
void DecompressAndLoadBgGfxUsingHeap(u8 bgId, const void *src, u32 size, u16 offset, u8 mode) {}
void DecompressAndLoadBgGfxUsingHeap2(u8 bgId, const void *src, u32 size, u16 offset, u8 mode) {}
void SetBgTilemapPalette(u8 bgId, u8 left, u8 top, u8 width, u8 height, u8 palette) {}
void CopyToBufferFromBgTilemap(u8 bgId, u16 *dest, u8 left, u8 top, u8 width, u8 height) {}
void ResetBgPositions(void) {}
void InitStandardTextBoxWindows(void) {}
void FreeAllOverworldWindowBuffers(void) {}
void InitTextBoxGfxAndPrinters(void) {}
void AddTextPrinterDiffStyle(bool8 allowSkippingDelayWithButtonPress) {}
void AddTextPrinterForMessage(bool8 allowSkippingDelayWithButtonPress) {}

bool8 FreeTempTileDataBuffersIfPossible(void) { return FALSE; }

// VRAM layout: each BG uses charBase and screenBase from BG control
// mode 0 = tiles (charBase), mode 1 = tilemap (screenBase)
// For now use fixed offsets per bgId
void *DecompressAndCopyTileDataToVram(u8 bgId, const void *src, u32 size, u16 offset, u8 mode)
{
    extern u8 gVRAM[];
    if (!src) return NULL;
    // Each BG char base is 0x4000 apart, screen base is 0x800 apart
    u8 *dest;
    if (mode == 0) // tiles
        dest = gVRAM + (bgId * 0x4000) + offset;
    else // tilemap
        dest = gVRAM + 0x10000 + (bgId * 0x800) + offset;
    LZ77UnCompWram(src, dest);
    return dest;
}
void *DecompressAndCopyTileDataToVram2(u8 bgId, const void *src, u32 size, u16 offset, u8 mode)
{
    return DecompressAndCopyTileDataToVram(bgId, src, size, offset, mode);
}
void *MallocAndDecompress(const void *src, u32 *size) { return NULL; }

u16 RunTextPrinters_CheckPrinter0Active(void) { return 0; }
u16 AddTextPrinterParameterized2(u8 windowId, u8 fontId, const u8 *str, u8 speed, void (*callback)(struct TextPrinterTemplate *, u16), u8 fgColor, u8 bgColor, u8 shadowColor) { return 0; }

#endif // PLATFORM_PC
