#ifndef GUARD_PC_PORT_H
#define GUARD_PC_PORT_H

#ifdef PLATFORM_PC

#include "gba/types.h"
#include "gba/defines.h"
#include <string.h>

// Override GBA memory section attributes with no-ops
#undef COMMON_DATA
#undef EWRAM_DATA
#undef IWRAM_DATA
#undef ALIGNED
#define COMMON_DATA
#define EWRAM_DATA
#define IWRAM_DATA
#define ALIGNED(n)

// Override INTR_CHECK and INTR_VECTOR with real variables
#undef INTR_CHECK
#undef INTR_VECTOR
extern u32 PC_INTR_CHECK;
extern u32 PC_INTR_VECTOR;
#define INTR_CHECK  PC_INTR_CHECK
#define INTR_VECTOR PC_INTR_VECTOR

// Override BG_PLTT with a real array
#undef BG_PLTT
extern u16 PC_BG_PLTT[256];
#define BG_PLTT ((u16 *)PC_BG_PLTT)

// GBA hardware register stubs
extern u16 PC_REG_DISPCNT;
extern u16 PC_REG_DISPSTAT;
extern u16 PC_REG_VCOUNT;
extern u16 PC_REG_WAITCNT;
extern u16 PC_REG_KEYINPUT;
extern u16 PC_REG_IME;
extern u16 PC_REG_IE;
extern u16 PC_REG_IF;
extern u16 PC_REG_TM1CNT_L;
extern u16 PC_REG_TM1CNT_H;

#define REG_DISPCNT    PC_REG_DISPCNT
#define REG_DISPSTAT   PC_REG_DISPSTAT
#define REG_VCOUNT     PC_REG_VCOUNT
#define REG_WAITCNT    PC_REG_WAITCNT
#define REG_KEYINPUT   PC_REG_KEYINPUT
#define REG_IME        PC_REG_IME
#define REG_IE         PC_REG_IE
#define REG_IF         PC_REG_IF
#define REG_TM1CNT_L   PC_REG_TM1CNT_L
#define REG_TM1CNT_H   PC_REG_TM1CNT_H

// GBA system call stubs
#define RegisterRamReset(x)     ((void)0)
#define SoftReset(x)            ((void)0)
#define DmaStop(x)              ((void)0)
#define DmaCopy32(ch,src,dst,s) memcpy(dst, src, s)
#define CpuFill16(val,dst,size) memset(dst, val, size)

#define CpuFill32(value, dest, size) memset(dest, value, size)
#define AGB_ASSERT_EX(exp, file, line) ((void)0)
#define ABSPATH(x) x

#endif // PLATFORM_PC
#endif // GUARD_PC_PORT_H