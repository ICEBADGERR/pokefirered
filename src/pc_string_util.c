#ifdef PLATFORM_PC

#include "global.h"
#include "string_util.h"
#include <string.h>

// GBA uses a custom character encoding - on PC we stub these
// They'll be properly implemented when we add text rendering

u8 gStringVar1[32] = {0};
u8 gStringVar2[20] = {0};
u8 gStringVar3[20] = {0};
u8 gStringVar4[20] = {0};

u8 *StringCopy(u8 *dest, const u8 *src)
{
    while ((*dest++ = *src++) != 0xFF);
    return dest - 1;
}
u8 *StringAppend(u8 *dest, const u8 *src)
{
    while (*dest != 0xFF) dest++;
    return StringCopy(dest, src);
}
u8 *StringCopyN(u8 *dest, const u8 *src, u8 n) { memcpy(dest, src, n); return dest + n; }
u8 *StringAppendN(u8 *dest, const u8 *src, u8 n) { return dest; }
u8 *StringCopy_Nickname(u8 *dest, const u8 *src) { return StringCopy(dest, src); }
u8 *StringGet_Nickname(u8 *str) { return str; }
u8 *StringCopy_PlayerName(u8 *dest, const u8 *src) { return StringCopy(dest, src); }
u8 *StringFill(u8 *dest, u8 c, u16 n) { memset(dest, c, n); return dest + n; }
u8 *StringFillWithTerminator(u8 *dest, u16 n) { memset(dest, 0xFF, n); return dest + n; }
u8 *StringCopyPadded(u8 *dest, const u8 *src, u8 c, u16 n) { return dest; }
u8 *StringExpandPlaceholders(u8 *dest, const u8 *src) { return StringCopy(dest, src); }
u8 *StringBraille(u8 *dest, const u8 *src) { return dest; }
u8 *GetExpandedPlaceholder(u32 id) { return gStringVar1; }
u8 *ConvertIntToDecimalStringN(u8 *dest, s32 value, enum StringConvertMode mode, u8 n) { return dest; }
u8 *ConvertUIntToDecimalStringN(u8 *dest, u32 value, enum StringConvertMode mode, u8 n) { return dest; }
u8 *ConvertIntToHexStringN(u8 *dest, s32 value, enum StringConvertMode mode, u8 n) { return dest; }
u8 *StringCopyN_Multibyte(u8 *dest, const u8 *src, u32 n) { return dest; }
u8 *WriteColorChangeControlCode(u8 *dest, u32 colorType, u8 color) { return dest; }
u16 StringLength(const u8 *str) { u16 i = 0; while (str[i] != 0xFF) i++; return i; }
u32 StringLength_Multibyte(const u8 *str) { return StringLength(str); }
s32 StringCompare(const u8 *str1, const u8 *str2) { return memcmp(str1, str2, StringLength(str1) + 1); }
s32 StringCompareN(const u8 *str1, const u8 *str2, u32 n) { return memcmp(str1, str2, n); }
s32 StringCompareWithoutExtCtrlCodes(const u8 *str1, const u8 *str2) { return StringCompare(str1, str2); }
bool8 IsStringLengthAtLeast(const u8 *str, s32 n) { return StringLength(str) >= n; }
bool32 IsStringJapanese(u8 *str) { return FALSE; }
u8 GetExtCtrlCodeLength(u8 code) { return 0; }
void ConvertInternationalString(u8 *s, u8 language) {}
void StripExtCtrlCodes(u8 *str) {}

#endif // PLATFORM_PC
