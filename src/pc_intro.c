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
#include "decompress.h"
#include "malloc.h"
#include <SDL2/SDL.h>
#include <stdio.h>
#include <stdlib.h>

extern u16 gFramebuffer[];
extern u8 gVRAM[];
#define GBA_WIDTH  240
#define GBA_HEIGHT 160

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

static u8 *PC_LoadFile(const char *path, u32 *size)
{
    FILE *f = fopen(path, "rb");
    if (!f) { SDL_Log("Failed to open %s", path); return NULL; }
    fseek(f, 0, SEEK_END);
    *size = ftell(f);
    fseek(f, 0, SEEK_SET);
    u8 *buf = (u8 *)malloc(*size);
    fread(buf, 1, *size, f);
    fclose(f);
    return buf;
}

static void LoadCopyrightGraphics(u32 charBase, u32 screenBase, u32 palOffset)
{
    u32 size;
    u8 *gfx = PC_LoadFile("graphics/intro/copyright.4bpp.lz", &size);
    u8 *map = PC_LoadFile("graphics/intro/copyright.bin.lz", &size);
    u8 *pal = PC_LoadFile("graphics/intro/copyright.gbapal", &size);
    SDL_Log("About to decompress gfx=%p map=%p pal=%p", (void*)gfx, (void*)map, (void*)pal);
    if (gfx) { SDL_Log("Decompressing GFX..."); LZ77UnCompWram(gfx, gVRAM + charBase); free(gfx); }
    if (map) { SDL_Log("Decompressing MAP..."); LZ77UnCompWram(map, gVRAM + screenBase); free(map); }

    if (pal) {
        memcpy(gPlttBufferFaded   + palOffset, pal, size);
        memcpy(gPlttBufferUnfaded + palOffset, pal, size);
        free(pal);
    }
    SDL_Log("LoadCopyrightGraphics done: charBase=%u screenBase=%u", charBase, screenBase);
    SDL_Log("VRAM[0..7]: %02X %02X %02X %02X %02X %02X %02X %02X",
        gVRAM[0], gVRAM[1], gVRAM[2], gVRAM[3],
        gVRAM[4], gVRAM[5], gVRAM[6], gVRAM[7]);
    SDL_Log("VRAM screenBase[0..3]: %02X %02X %02X %02X",
        gVRAM[7*0x800], gVRAM[7*0x800+1], gVRAM[7*0x800+2], gVRAM[7*0x800+3]);
    SDL_Log("Palette[0]=%04X [1]=%04X [2]=%04X", gPlttBufferFaded[0], gPlttBufferFaded[1], gPlttBufferFaded[2]);
}

static void CB2_WaitFadeBeforeSetUpIntro(void);
static void CB2_SetUpIntro(void);
static void CB2_Intro(void);

static void CB2_WaitFadeBeforeSetUpIntro(void)
{
    if (!UpdatePaletteFade())
    {
        SDL_Log("-> CB2_SetUpIntro");
        SetMainCallback2(CB2_SetUpIntro);
    }
}

static void CB2_SetUpIntro(void)
{
    switch (gMain.state)
    {
    default:
        gMain.state = 0;
    case 0:
        SDL_Log("CB2_SetUpIntro state 0");
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
        SDL_Log("CB2_SetUpIntro state 1");
        gMain.state++;
        break;
    case 2:
        SDL_Log("CB2_SetUpIntro state 2 -> CB2_Intro");
        BlendPalettes(PALETTES_ALL, 16, RGB_BLACK);
        SetMainCallback2(CB2_Intro);
        SetVBlankCallback(VBlankCB_Intro);
        return;
    }
}

extern void PC_RenderFrame(void);

static void CB2_Intro(void)
{
    static int frameCount = 0;
    if (frameCount == 0)
    {
        SDL_Log("CB2_Intro running!");
        // Check if VRAM has data
        u32 nonzero = 0;
        for (int i = 0; i < 1248; i++)
            if (gVRAM[i]) nonzero++;
        SDL_Log("VRAM non-zero bytes in first 1248: %u", nonzero);
        // Check palette
        SDL_Log("Palette[0]=%04X Palette[1]=%04X", gPlttBufferFaded[0], gPlttBufferFaded[1]);
    }
    frameCount++;
    RunTasks();
    AnimateSprites();
    BuildOamBuffer();
    UpdatePaletteFade();
    PC_RenderFrame();
}

static bool8 SetUpCopyrightScreen(void)
{
    switch (gMain.state)
    {
    case 0:
        SDL_Log("SetUpCopyrightScreen state 0");
        SetVBlankCallback(NULL);
        SetGpuReg(REG_OFFSET_DISPCNT, 0);
        ResetPaletteFade();
        LoadCopyrightGraphics(0, 7 * 0x800, 0);
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
        SDL_Log("SetUpCopyrightScreen -> CB2_WaitFadeBeforeSetUpIntro");
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
