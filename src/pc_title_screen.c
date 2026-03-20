#ifdef PLATFORM_PC

#include "global.h"
#include "title_screen.h"
#include "main.h"
#include "malloc.h"
#include "decompress.h"
#include "palette.h"
#include "sprite.h"
#include "task.h"
#include "scanline_effect.h"
#include "gpu_regs.h"
#include "new_menu_helpers.h"
#include "bg.h"
#include "sound.h"
#include <SDL2/SDL.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

extern u8 gVRAM[];
extern u16 gFramebuffer[];

#define GBA_WIDTH  240
#define GBA_HEIGHT 160

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

static void PC_LoadPalFileSize(const char *path, u16 palOffset, u32 maxSize)
{
    u32 size;
    u8 *pal = PC_LoadFile(path, &size);
    if (pal) {
        if (size > maxSize) size = maxSize;
        memcpy(gPlttBufferFaded   + palOffset, pal, size);
        memcpy(gPlttBufferUnfaded + palOffset, pal, size);
        free(pal);
        SDL_Log("Loaded palette (capped): %s offset=%u size=%u", path, palOffset, size);
    }
}

static void PC_LoadPalFile(const char *path, u16 palOffset)
{
    u32 size;
    u8 *pal = PC_LoadFile(path, &size);
    if (pal) {
        memcpy(gPlttBufferFaded   + palOffset, pal, size);
        memcpy(gPlttBufferUnfaded + palOffset, pal, size);
        free(pal);
        SDL_Log("Loaded palette: %s offset=%u size=%u", path, palOffset, size);
    }
}

// Screen base offsets matching sBgTemplates: BG0=31, BG1=30, BG2=29, BG3=28
static const u8 sScreenBases[] = {31, 30, 29, 28};

static void PC_LoadTiles(const char *path, u8 bgId)
{
    u32 size;
    u8 *data = PC_LoadFile(path, &size);
    if (data) {
        u8 *dest = gVRAM + (bgId * 0x4000);
        LZ77UnCompWram(data, dest);
        free(data);
        SDL_Log("Loaded tiles: %s bgId=%u at VRAM offset 0x%X", path, bgId, bgId * 0x4000);
    }
}

static void PC_LoadTilemap(const char *path, u8 bgId)
{
    u32 size;
    u8 *data = PC_LoadFile(path, &size);
    if (data) {
        u32 offset = sScreenBases[bgId] * 0x800;
        u8 *dest = gVRAM + offset;
        LZ77UnCompWram(data, dest);
        free(data);
        SDL_Log("Loaded tilemap: %s bgId=%u at VRAM offset 0x%X", path, bgId, offset);
    }
}

static void VBlankCB(void) {}
static u8 sTitleScreenTimerTaskId = 0xFF;

static void Task_TitleScreenMain(u8 taskId) {}
static void Task_TitleScreenTimer(u8 taskId) {}
static void CB2_TitleScreenRun(void);

extern void PC_RenderFrame(void);

static void CB2_TitleScreenRun(void)
{
    RunTasks();
    AnimateSprites();
    BuildOamBuffer();
    UpdatePaletteFade();
    PC_RenderFrame();
}

void CB2_InitTitleScreen(void)
{
    switch (gMain.state)
    {
    default:
        gMain.state = 0;
    case 0:
        SDL_Log("CB2_InitTitleScreen state 0");
        SetVBlankCallback(NULL);
        InitHeap(gHeap, HEAP_SIZE);
        ResetTasks();
        ResetSpriteData();
        FreeAllSpritePalettes();
        ResetPaletteFade();
        memset(gVRAM, 0, 0x18000);
        ResetBgsAndClearDma3BusyFlags(FALSE);
        sTitleScreenTimerTaskId = 0xFF;
        gMain.state = 1;
        return;
    case 1:
        SDL_Log("CB2_InitTitleScreen state 1 - loading graphics");
        // BG0 - Game title logo (8bpp) - uses 13 palettes starting at 0
        // Load only 13*32=416 bytes (13 palettes x 16 colors x 2 bytes)
        PC_LoadPalFileSize("graphics/title_screen/firered/game_title_logo.gbapal", 0, 13 * 32);
        PC_LoadTiles("graphics/title_screen/firered/game_title_logo.8bpp.lz", 0);
        PC_LoadTilemap("graphics/title_screen/firered/game_title_logo.bin.lz", 0);
        // BG1 - Box art mon (Charizard) - palette 13
        PC_LoadPalFile("graphics/title_screen/firered/box_art_mon.gbapal", 13 * 16);
        PC_LoadTiles("graphics/title_screen/firered/box_art_mon.4bpp.lz", 1);
        PC_LoadTilemap("graphics/title_screen/firered/box_art_mon.bin.lz", 1);
        // BG2 - Copyright/Press Start - palette 15
        PC_LoadPalFile("graphics/title_screen/firered/background.gbapal", 15 * 16);
        PC_LoadTiles("graphics/title_screen/copyright_press_start.4bpp.lz", 2);
        PC_LoadTilemap("graphics/title_screen/copyright_press_start.bin.lz", 2);
        // BG3 - Border - palette 14
        PC_LoadPalFile("graphics/title_screen/firered/background.gbapal", 14 * 16);
        PC_LoadTiles("graphics/title_screen/border_bg.4bpp.lz", 3);
        PC_LoadTilemap("graphics/title_screen/firered/border_bg.bin.lz", 3);
        gMain.state++;
        break;
    case 2:
        // Set backdrop color to sky color (palette 15, color 15 = orange-red)
        gPlttBufferFaded[0] = gPlttBufferFaded[15 * 16 + 15];
        gPlttBufferUnfaded[0] = gPlttBufferUnfaded[15 * 16 + 15];
        SDL_Log("CB2_InitTitleScreen state 2 -> CB2_TitleScreenRun, backdrop=0x%04X", gPlttBufferFaded[0]);
        BlendPalettes(PALETTES_BG, 16, RGB_BLACK);
        CreateTask(Task_TitleScreenMain, 4);
        SetVBlankCallback(VBlankCB);
        SetMainCallback2(CB2_TitleScreenRun);
        return;
    }
}

#endif // PLATFORM_PC
