#ifdef PLATFORM_PC
#include "global.h"
#include "gpu_regs.h"

void InitGpuRegManager(void) {}
void CopyBufferedValuesToGpuRegs(void) {}
void SetGpuReg(u8 regOffset, u16 value) {}
void SetGpuReg_ForcedBlank(u8 regOffset, u16 value) {}
u16 GetGpuReg(u8 regOffset) { return 0; }
void SetGpuRegBits(u8 regOffset, u16 mask) {}
void ClearGpuRegBits(u8 regOffset, u16 mask) {}
void EnableInterrupts(u16 mask) {}
void DisableInterrupts(u16 mask) {}

#endif // PLATFORM_PC
