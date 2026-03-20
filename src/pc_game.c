#ifdef PLATFORM_PC

#include "global.h"
#include "main.h"
#include "malloc.h"
#include "intro.h"

MainCallback gallback1 = NULL;
MainCallback gallback2 = NULL;
struct Main gMain = {0};
struct SaveBlock2 *gSaveBlock2Ptr = NULL;
struct SaveBlock1 *gSaveBlock1Ptr = NULL;

u8 gHeap[HEAP_SIZE];

void SetMainCallback2(MainCallback callback)
{
    gallback2 = callback;
    gMain.state = 0;
}

void InitMainCallbacks(void)
{
    gMain.vblankCounter1 = 0;
    gMain.vblankCounter2 = 0;
    gallback1 = NULL;
    SetMainCallback2(CB2_InitCopyrightScreenAfterBootup);
}

void PC_CallCallbacks(void)
{
    if (gallback1)
        gallback1();
    if (gallback2)
        gallback2();
}

// Stub out CB2_InitCopyrightScreenAfterBootup for now
// Draws a blue screen so we know it's being called
extern u16 gFramebuffer[];
#define GBA_WIDTH  240
#define GBA_HEIGHT 160

void CB2_InitCopyrightScreenAfterBootup(void)
{
    // blue in RGB555: b=31
    for (int i = 0; i < GBA_WIDTH * GBA_HEIGHT; i++)
        gFramebuffer[i] = (31 << 10);

    SetMainCallback2(CB2_InitCopyrightScreenAfterBootup);
}

#endif // PLATFORM_PC
