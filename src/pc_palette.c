#ifdef PLATFORM_PC

#include "global.h"
#include "palette.h"

// Palette buffers - these will eventually feed into our RGB555 renderer
u16 gPlttBufferUnfaded[PLTT_BUFFER_SIZE] = {0};
u16 gPlttBufferFaded[PLTT_BUFFER_SIZE] = {0};
u32 gPlttBufferTransferPending = 0;
struct PaletteFadeControl gPaletteFade = {0};

void LoadCompressedPalette(const u32 *src, u16 offset, u16 size) {}
void LoadPalette(const void *src, u16 offset, u16 size)
{
    // Actually copy palette data so colors are available for rendering
    if (offset + size <= PLTT_BUFFER_SIZE * 2)
        memcpy((u8 *)gPlttBufferUnfaded + offset, src, size);
}
void FillPalette(u16 value, u16 offset, u16 size)
{
    u16 i;
    for (i = 0; i < size / 2; i++)
        gPlttBufferUnfaded[offset / 2 + i] = value;
}
void TransferPlttBuffer(void) {}
u8 UpdatePaletteFade(void) { return 0; }
void ResetPaletteFade(void) {}
void ReadPlttIntoBuffers(void) {}
bool8 BeginNormalPaletteFade(u32 selectedPalettes, s8 delay, u8 startY, u8 targetY, u16 blendColor) { return FALSE; }
void ResetPaletteFadeControl(void) {}
void InvertPlttBuffer(u32 selectedPalettes) {}
void TintPlttBuffer(u32 selectedPalettes, s8 r, s8 g, s8 b) {}
void UnfadePlttBuffer(u32 selectedPalettes) {}
void BeginFastPaletteFade(u8 submode) {}
void BeginHardwarePaletteFade(u8 blendCnt, u8 delay, u8 y, u8 targetY, u8 shouldResetBlendRegisters) {}
void BlendPalettes(u32 selectedPalettes, u8 coeff, u16 color) {}
void BlendPalettesUnfaded(u32 selectedPalettes, u8 coeff, u16 color) {}
void TintPalette_GrayScale(u16 *palette, u16 count) {}
void TintPalette_GrayScale2(u16 *palette, u16 count) {}
void TintPalette_SepiaTone(u16 *palette, u16 count) {}
void TintPalette_CustomTone(u16 *palette, u16 count, u16 rTone, u16 gTone, u16 bTone) {}
void PaletteStruct_ResetById(u16 id) {}
void CopyPaletteInvertedTint(const u16 *src, u16 *dst, u16 count, u8 tone) {}
void BlendPalettesGradually(u32 selectedPalettes, s8 delay, u8 coeff, u8 coeffTarget, u16 color, u8 priority, u8 id) {}
bool32 IsBlendPalettesGraduallyTaskActive(u8 var) { return FALSE; }
void DestroyBlendPalettesGraduallyTask(void) {}

#endif // PLATFORM_PC
