#ifdef PLATFORM_PC
#include "global.h"
#include "scanline_effect.h"

struct ScanlineEffect gScanlineEffect = {0};
u16 gScanlineEffectRegBuffers[2][0x3C0] = {0};

void ScanlineEffect_Stop(void) {}
void ScanlineEffect_Clear(void) {}
void ScanlineEffect_SetParams(struct ScanlineEffectParams params) {}
void ScanlineEffect_InitHBlankDmaTransfer(void) {}
u8 ScanlineEffect_InitWave(u8 startLine, u8 endLine, u8 frequency, u8 amplitude, u8 delayInterval, u8 regOffset, bool8 a7) { return 0; }

#endif // PLATFORM_PC
