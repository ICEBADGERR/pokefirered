#ifdef PLATFORM_PC

#include "global.h"
#include "window.h"

u8 gWindowClearTile = 0;
void *gWindowBgTilemapBuffers[4] = {0};

bool16 InitWindows(const struct WindowTemplate *templates) { return TRUE; }
u16 AddWindow(const struct WindowTemplate *template) { return 0; }
void RemoveWindow(u8 windowId) {}
void FreeAllWindowBuffers(void) {}
void CopyWindowToVram(u8 windowId, u8 mode) {}
void CopyWindowToVram8Bit(u8 windowId, u8 mode) {}
void PutWindowTilemap(u8 windowId) {}
void PutWindowRectTilemapOverridePalette(u8 windowId, u8 x, u8 y, u8 width, u8 height, u8 palette) {}
void ClearWindowTilemap(u8 windowId) {}
void PutWindowRectTilemap(u8 windowId, u8 x, u8 y, u8 width, u8 height) {}
void BlitBitmapToWindow(u8 windowId, const u8 *pixels, u16 x, u16 y, u16 width, u16 height) {}
void BlitBitmapRectToWindow(u8 windowId, const u8 *pixels, u16 srcX, u16 srcY, u16 srcWidth, int srcHeight, u16 destX, u16 destY, u16 rectWidth, u16 rectHeight) {}
void FillWindowPixelRect(u8 windowId, u8 fillValue, u16 x, u16 y, u16 width, u16 height) {}
void CopyToWindowPixelBuffer(u8 windowId, const void *src, u16 size, u16 tileOffset) {}
void FillWindowPixelBuffer(u8 windowId, u8 fillValue) {}
void ScrollWindow(u8 windowId, u8 direction, u8 distance, u8 fillValue) {}
void CallWindowFunction(u8 windowId, WindowFunc func) {}
bool8 SetWindowAttribute(u8 windowId, u8 attributeId, u32 value) { return FALSE; }
u32 GetWindowAttribute(u8 windowId, u8 attributeId) { return 0; }
void LoadUserWindowBorderGfx(u8 windowId, u8 tileOffset, u8 paletteOffset) {}
void DrawStdWindowFrame(u8 windowId, bool8 copyToVram) {}
void DrawStdFrameWithCustomTileAndPalette(u8 windowId, bool8 copyToVram, u8 tileOffset, u8 paletteNum) {}
void ClearStdWindowAndFrame(u8 windowId, bool8 copyToVram) {}
void SetDefaultFontsPointer(void) {}

#endif // PLATFORM_PC
