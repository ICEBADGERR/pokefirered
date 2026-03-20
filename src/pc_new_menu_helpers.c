#ifdef PLATFORM_PC

#include "global.h"
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

void *DecompressAndCopyTileDataToVram(u8 bgId, const void *src, u32 size, u16 offset, u8 mode) { return NULL; }
void *DecompressAndCopyTileDataToVram2(u8 bgId, const void *src, u32 size, u16 offset, u8 mode) { return NULL; }
void *MallocAndDecompress(const void *src, u32 *size) { return NULL; }

u16 RunTextPrinters_CheckPrinter0Active(void) { return 0; }
u16 AddTextPrinterParameterized2(u8 windowId, u8 fontId, const u8 *str, u8 speed, void (*callback)(struct TextPrinterTemplate *, u16), u8 fgColor, u8 bgColor, u8 shadowColor) { return 0; }

#endif // PLATFORM_PC
