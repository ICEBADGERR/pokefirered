#ifdef PLATFORM_PC

#include "global.h"
#include "intro.h"
#include "main.h"
#include "save.h"
#include "new_game.h"

extern u16 gFramebuffer[];
#define GBA_WIDTH  240
#define GBA_HEIGHT 160

// Stub copyright screen - just shows white then transitions
// Will be replaced with real intro sequence later
void CB2_InitCopyrightScreenAfterBootup(void)
{
    // Fill with white to signal we're alive
    u16 i;
    for (i = 0; i < GBA_WIDTH * GBA_HEIGHT; i++)
        gFramebuffer[i] = 0x7FFF;

    SetMainCallback2(CB2_InitCopyrightScreenAfterBootup);
}

void CB2_InitCopyrightScreenAfterTitleScreen(void)
{
    SetMainCallback2(CB2_InitCopyrightScreenAfterTitleScreen);
}

#endif // PLATFORM_PC
