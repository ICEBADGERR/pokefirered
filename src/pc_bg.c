#ifdef PLATFORM_PC

#include "global.h"
#include "bg.h"

void ResetBgs(void) {}
void ResetBgControlStructs(void) {}
void Unused_ResetBgControlStruct(u8 bg) {}
void SetTextModeAndHideBgs(void) {}
void ResetBgsAndClearDma3BusyFlags(bool32 enableWindowTileAutoAlloc) {}
void InitBgsFromTemplates(u8 bgMode, const struct BgTemplate *templates, u8 numTemplates) {}
void InitBgFromTemplate(const struct BgTemplate *template) {}
void SetBgMode(u8 bgMode) {}
void ShowBg(u8 bg) {}
void HideBg(u8 bg) {}
void SetBgAttribute(u8 bg, u8 attributeId, u8 value) {}
void SetBgAffine(u8 bg, u32 srcCenterX, u32 srcCenterY, s16 dispCenterX, s16 dispCenterY, s16 scaleX, s16 scaleY, u16 rotationAngle) {}
u8 AdjustBgMosaic(u8 value, u8 mode) { return 0; }
u8 GetBgMode(void) { return 0; }
u8 LoadBgVram(u8 bg, const void *src, u16 size, u16 destOffset, u8 mode) { return 0; }
u16 GetBgControlAttribute(u8 bg, u8 attributeId) { return 0; }
u16 LoadBgTiles(u8 bg, const void *src, u16 size, u16 destOffset) { return 0; }
u16 LoadBgTilemap(u8 bg, const void *src, u16 size, u16 destOffset) { return 0; }
u16 Unused_LoadBgPalette(u8 bg, const void *src, u16 size, u16 destOffset) { return 0; }
u16 GetBgAttribute(u8 bg, u8 attributeId) { return 0; }
u32 ChangeBgX(u8 bg, u32 value, u8 op) { return 0; }
u32 GetBgX(u8 bg) { return 0; }
u32 ChangeBgY(u8 bg, u32 value, u8 op) { return 0; }
u32 ChangeBgY_ScreenOff(u8 bg, u32 value, u8 op) { return 0; }
u32 GetBgY(u8 bg) { return 0; }
bool8 IsInvalidBg(u8 bg) { return FALSE; }
bool8 IsDma3ManagerBusyWithBgCopy(void) { return FALSE; }
void SetBgControlAttributes(u8 bg, u8 charBaseIndex, u8 mapBaseIndex, u8 screenSize, u8 paletteMode, u8 priority, u8 mosaic, u8 wraparound) {}

#endif // PLATFORM_PC
