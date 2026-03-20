#ifdef PLATFORM_PC

#include "global.h"
#include "main.h"

// Globals needed by the callback system
MainCallback gallback1 = NULL;
MainCallback gallback2 = NULL;
struct Main gMain = {0};
struct SaveBlock2 *gSaveBlock2Ptr = NULL;
struct SaveBlock1 *gSaveBlock1Ptr = NULL;

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
    // Don't call CB2_InitCopyrightScreenAfterBootup yet
    // SetMainCallback2(CB2_InitCopyrightScreenAfterBootup);
}

void PC_CallCallbacks(void)
{
    if (gallback1)
        gallback1();
    if (gallback2)
        gallback2();
}

#endif // PLATFORM_PC

#ifdef PLATFORM_PC
// Test callback - draws a red screen to prove the callback system works
extern u16 gFramebuffer[];
#define GBA_WIDTH  240
#define GBA_HEIGHT 160

static void TestCallback(void)
{
    // Fill framebuffer with red (RGB555: r=31, g=0, b=0)
    for (int i = 0; i < GBA_WIDTH * GBA_HEIGHT; i++)
        gFramebuffer[i] = 31; // pure red in RGB555

    // Register ourselves again next frame
    SetMainCallback2(TestCallback);
}

void PC_RegisterTestCallback(void)
{
    SetMainCallback2(TestCallback);
}
#endif
