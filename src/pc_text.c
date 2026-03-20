#ifdef PLATFORM_PC

#include "global.h"
#include "text.h"

void SetFontsPointer(const struct FontInfo *fonts) {}
void DeactivateAllTextPrinters(void) {}
void RunTextPrinters(void) {}
void GenerateFontHalfRowLookupTable(u8 fgColor, u8 bgColor, u8 shadowColor) {}
void SaveTextColors(u8 *fgColor, u8 *bgColor, u8 *shadowColor) {}
void RestoreTextColors(u8 *fgColor, u8 *bgColor, u8 *shadowColor) {}
void DecompressGlyphTile(const u16 *src, u16 *dest) {}
void CopyGlyphToWindow(struct TextPrinter *x) {}
void ClearTextSpan(struct TextPrinter *textPrinter, u32 width) {}
u16 AddTextPrinterParameterized(u8 windowId, u8 fontId, const u8 *str, u8 x, u8 y, u8 speed, void (*callback)(struct TextPrinterTemplate *, u16)) { return 0; }
bool16 AddTextPrinter(struct TextPrinterTemplate *textSubPrinter, u8 speed, void (*callback)(struct TextPrinterTemplate *, u16)) { return FALSE; }
bool16 IsTextPrinterActive(u8 id) { return FALSE; }
u32 RenderFont(struct TextPrinter *textPrinter) { return 0; }
u8 GetLastTextColor(u8 colorType) { return 0; }

#endif // PLATFORM_PC
