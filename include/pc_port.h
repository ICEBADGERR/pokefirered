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


// DMA + REG_ADDR constants needed by scanline_effect.h
#define REG_ADDR_BG0HOFS 0
#define REG_ADDR_BG0VOFS 2
#define REG_ADDR_BG1HOFS 4
#define REG_ADDR_BG1VOFS 6
#define REG_ADDR_BG2HOFS 8
#define REG_ADDR_BG2VOFS 10
#define REG_ADDR_BG3HOFS 12
#define REG_ADDR_BG3VOFS 14
#define DMA_ENABLE        0x8000
#define DMA_START_HBLANK  0x2000
#define DMA_START_VBLANK  0x1000
#define DMA_REPEAT        0x0200
#define DMA_SRC_INC       0x0000
#define DMA_DEST_INC      0x0000
#define DMA_DEST_RELOAD   0x0060
#define DMA_16BIT         0x0000
#define DMA_32BIT         0x0400

// VRAM/OAM stubs
#undef VRAM
#define VRAM ((u32)(uintptr_t)gVRAM)
#undef OAM
#define OAM ((u32)(uintptr_t)gOAM)
#undef VRAM_SIZE
#define VRAM_SIZE PC_VRAM_SIZE
#undef OAM_SIZE
#define OAM_SIZE PC_OAM_SIZE
#define DmaFill16(ch, val, dst, size) memset((void*)(dst), val, size)
#define DmaFill32(ch, val, dst, size) memset((void*)(dst), val, size)

#define PC_VRAM_SIZE 0x18000
#define PC_OAM_SIZE  0x400
#define PC_PLTT_SIZE 0x400

extern u8 gVRAM[PC_VRAM_SIZE];
extern u8 gOAM[PC_OAM_SIZE];
extern u8 gPLTT[PC_PLTT_SIZE];

// REG_OFFSET constants for display registers
#define REG_OFFSET_DISPCNT  0x0
#define REG_OFFSET_DISPSTAT 0x4
#define REG_OFFSET_BLDCNT   0x50
#define REG_OFFSET_BLDALPHA 0x52
#define REG_OFFSET_BLDY     0x54
#define REG_OFFSET_BG0CNT   0x8
#define REG_OFFSET_BG0HOFS  0x10
#define REG_OFFSET_BG0VOFS  0x12

#endif // PLATFORM_PC
#endif // GUARD_PC_PORT_H