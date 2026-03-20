#ifdef PLATFORM_PC

#include "global.h"
#include "intro.h"
#include "main.h"
#include "save.h"
#include "new_game.h"
#include "palette.h"
#include "sprite.h"
#include "task.h"
#include "scanline_effect.h"
#include "gpu_regs.h"
#include "new_menu_helpers.h"
#include "bg.h"
#include "malloc.h"

extern u16 gFramebuffer[];
#define GBA_WIDTH  240
#define GBA_HEIGHT 160

// GameCube multiboot stubs
typedef struct { u8 gcmb_field_2; } GcmbStruct;
static GcmbStruct sGcmb = {0};
static void GameCubeMultiBoot_Init(GcmbStruct *s) {}
static void GameCubeMultiBoot_Main(GcmbStruct *s) {}
static void GameCubeMultiBoot_Quit(void) {}
static void ResetSerial(void) {}
static void SerialCB(void) {}
static void SerialCB_CopyrightScreen(void) {}
static void VBlankCB_Copyright(void) {}
static void VBlankCB_Intro(void) {}
static void LoadCopyrightGraphics(u32 a, u32 b, u32 c) {}

static void CB2_WaitFadeBeforeSetUpIntro(void);
static void CB2_SetUpIntro(void);
static void CB2_Intro(void);

static void CB2_WaitFadeBeforeSetUpIntro(void)
{
    if (!UpdatePaletteFade())
        SetMainCallback2(CB2_SetUpIntro);
}

static void CB2_SetUpIntro(void)
{
    switch (gMain.state)
    {
    default:
        gMain.state = 0;
        // fallthrough
    case 0:
        SetVBlankCallback(NULL);
        SetGpuReg(REG_OFFSET_DISPCNT, 0);
        InitHeap(gHeap, HEAP_SIZE);
        ResetTasks();
        ResetSpriteData();
        ResetPaletteFade();
        ResetTempTileDataBuffers();
        ScanlineEffect_Stop();
        FreeAllSpritePalettes();
        ResetBgsAndClearDma3BusyFlags(FALSE);
        gMain.state++;
        break;
    case 1:
        // Skip loading GF logo graphics for now
        gMain.state++;
        break;
    case 2:
        // Skip waiting for DMA
        BlendPalettes(PALETTES_ALL, 16, RGB_BLACK);
        SetMainCallback2(CB2_Intro);
        SetVBlankCallback(VBlankCB_Intro);
        return;
    }
}

static void CB2_Intro(void)
{
    RunTasks();
    AnimateSprites();
    BuildOamBuffer();
    UpdatePaletteFade();

    // Fill with a color to show we've reached CB2_Intro
    u16 i;
    for (i = 0; i < GBA_WIDTH * GBA_HEIGHT; i++)
        gFramebuffer[i] = 0x001F; // red in RGB555
}

static bool8 SetUpCopyrightScreen(void)
{
    switch (gMain.state)
    {
    case 0:
        SetVBlankCallback(NULL);
        SetGpuReg(REG_OFFSET_DISPCNT, 0);
        ResetPaletteFade();
        LoadCopyrightGraphics(0, 0, 0);
        ScanlineEffect_Stop();
        ResetTasks();
        ResetSpriteData();
        FreeAllSpritePalettes();
        BeginNormalPaletteFade(PALETTES_ALL, 0, 16, 0, RGB_WHITEALPHA);
        SetVBlankCallback(VBlankCB_Copyright);
        gMain.state++;
        break;
    default:
        UpdatePaletteFade();
        gMain.state++;
        if (gMain.state >= 140)
            gMain.state = 142;
        break;
    case 142:
        SetMainCallback2(CB2_WaitFadeBeforeSetUpIntro);
        return FALSE;
    }
    return TRUE;
}

void CB2_InitCopyrightScreenAfterBootup(void)
{
    if (!SetUpCopyrightScreen())
    {
        ResetMenuAndMonGlobals();
        Save_ResetSaveCounters();
        LoadGameSave(SAVE_NORMAL);
        if (gSaveFileStatus == SAVE_STATUS_EMPTY || gSaveFileStatus == SAVE_STATUS_INVALID)
            Sav2_ClearSetDefault();
    }
}

void CB2_InitCopyrightScreenAfterTitleScreen(void)
{
    SetUpCopyrightScreen();
}

#endif // PLATFORM_PC
