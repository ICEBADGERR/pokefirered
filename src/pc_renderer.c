#ifdef PLATFORM_PC
#ifndef USE_OPENGL

#include "global.h"
#include "palette.h"
#include <SDL2/SDL.h>

#define GBA_WIDTH   240
#define GBA_HEIGHT  160
#define TILE_SIZE   8
#define TILES_X     (GBA_WIDTH / TILE_SIZE)   // 30
#define TILES_Y     (GBA_HEIGHT / TILE_SIZE)  // 20

extern u8 gVRAM[];
extern u16 gFramebuffer[];

// Render a 4bpp BG layer
static void PC_RenderBG_4bpp(u8 charBase, u8 screenBase)
{
    u8  *tileData = gVRAM + (charBase * 0x4000);
    u16 *tileMap  = (u16 *)(gVRAM + (screenBase * 0x800));

    for (int ty = 0; ty < TILES_Y; ty++)
    {
        for (int tx = 0; tx < TILES_X; tx++)
        {
            u16 entry      = tileMap[ty * 32 + tx];
            u16 tileIndex  = entry & 0x3FF;
            u8  paletteNum = (entry >> 12) & 0xF;
            u8 *tile = tileData + (tileIndex * 32);
            u16 *pal = gPlttBufferFaded + (paletteNum * 16);

            for (int py = 0; py < TILE_SIZE; py++)
            {
                for (int px = 0; px < TILE_SIZE; px++)
                {
                    u8 byte = tile[(py * 4) + (px / 2)];
                    u8 colorIndex = (px & 1) ? (byte >> 4) : (byte & 0xF);
                    int screenX = tx * TILE_SIZE + px;
                    int screenY = ty * TILE_SIZE + py;
                    if (colorIndex == 0)
                        gFramebuffer[screenY * GBA_WIDTH + screenX] = gPlttBufferFaded[0];
                    else
                        gFramebuffer[screenY * GBA_WIDTH + screenX] = pal[colorIndex];
                }
            }
        }
    }
}

// Render an 8bpp BG layer
static void PC_RenderBG_8bpp(u8 charBase, u8 screenBase)
{
    u8  *tileData = gVRAM + (charBase * 0x4000);
    u16 *tileMap  = (u16 *)(gVRAM + (screenBase * 0x800));

    for (int ty = 0; ty < TILES_Y; ty++)
    {
        for (int tx = 0; tx < TILES_X; tx++)
        {
            u16 entry     = tileMap[ty * 32 + tx];
            u16 tileIndex = entry & 0x3FF;
            u8 *tile = tileData + (tileIndex * 64);

            for (int py = 0; py < TILE_SIZE; py++)
            {
                for (int px = 0; px < TILE_SIZE; px++)
                {
                    u8 colorIndex = tile[py * 8 + px];
                    int screenX = tx * TILE_SIZE + px;
                    int screenY = ty * TILE_SIZE + py;
                    // In 8bpp mode, color 0 is transparent (show what is behind)
                    // Don't overwrite existing pixel if colorIndex is 0
                    if (colorIndex != 0)
                        gFramebuffer[screenY * GBA_WIDTH + screenX] = gPlttBufferFaded[colorIndex];
                }
            }
        }
    }
}

// Screen base offsets matching sBgTemplates
// charBase N = VRAM offset N*0x4000
// mapBase N  = VRAM offset N*0x800
// BG0: char=0, map=31  BG1: char=1, map=30  BG2: char=2, map=29  BG3: char=3, map=28
// Sprite size lookup: [shape][size] = {width, height} in pixels
static const u8 sSpriteWidths[3][4] = {
    {8, 16, 32, 64}, // square
    {16, 32, 32, 64}, // horizontal
    {8,  8, 16, 32}  // vertical
};
static const u8 sSpriteHeights[3][4] = {
    {8, 16, 32, 64}, // square
    {8,  8, 16, 32}, // horizontal
    {16, 32, 32, 64} // vertical
};

extern struct OamData gOamBuffer[];

static void PC_RenderSprites(void)
{
    for (int i = 127; i >= 0; i--) // render back to front
    {
        struct OamData *oam = &gOamBuffer[i];
        int y = oam->y;
        if (y >= 160) continue; // offscreen/hidden

        int x = oam->x;
        if (x >= 240 && x < 0x1F0) continue; // offscreen

        u8 shape = oam->shape;
        u8 size  = oam->size;
        if (shape > 2 || size > 3) continue;

        int w = sSpriteWidths[shape][size];
        int h = sSpriteHeights[shape][size];
        int bpp8 = (oam->bpp == 1);
        u16 tileNum = oam->tileNum;
        u8 palNum = oam->paletteNum;

        // OBJ tiles start at VRAM 0x10000, each tile is 32 bytes (4bpp) or 64 bytes (8bpp)
        u8 *objVram = gVRAM + 0x10000;
        u16 *pal = gPlttBufferFaded + 256 + (palNum * 16);

        int tilesPerRow = w / 8;

        for (int py = 0; py < h; py++)
        {
            int screenY = y + py;
            if (screenY < 0 || screenY >= 160) continue;

            for (int px = 0; px < w; px++)
            {
                int screenX = x + px;
                if (screenX < 0 || screenX >= 240) continue;

                // Which tile are we in?
                int tileX = px / 8;
                int tileY = py / 8;
                int tileOffset = tileNum + tileY * tilesPerRow + tileX;

                u8 colorIndex;
                if (bpp8)
                {
                    u8 *tile = objVram + (tileOffset * 64);
                    colorIndex = tile[(py % 8) * 8 + (px % 8)];
                    if (colorIndex == 0) continue;
                    gFramebuffer[screenY * GBA_WIDTH + screenX] = gPlttBufferFaded[256 + colorIndex];
                }
                else
                {
                    u8 *tile = objVram + (tileOffset * 32);
                    u8 byte = tile[(py % 8) * 4 + (px % 8) / 2];
                    colorIndex = (px % 2) ? (byte >> 4) : (byte & 0xF);
                    if (colorIndex == 0) continue;
                    gFramebuffer[screenY * GBA_WIDTH + screenX] = pal[colorIndex];
                }
            }
        }
    }
}

static int sDebugFrame = 0;
void PC_RenderFrame(void)
{
    // Clear to backdrop color (palette[0])
    u16 backdrop = gPlttBufferFaded[0];
    for (int i = 0; i < GBA_WIDTH * GBA_HEIGHT; i++)
        gFramebuffer[i] = backdrop;

    // Debug: check BG2 tilemap every frame until we find when it goes zero
    {
        u16 *tm = (u16 *)(gVRAM + 29 * 0x800);
        static u16 lastVal = 0;
        u16 curVal = tm[18*32+5];
        if (curVal != lastVal) {
            SDL_Log("Frame %d: BG2 row18 col5 CHANGED to %04X", sDebugFrame, curVal);
            lastVal = curVal;
        }
        sDebugFrame++;
    }
    // Render BG layers back to front using correct char/screen bases
    PC_RenderBG_4bpp(3, 28); // BG3 border (lowest priority)
    PC_RenderBG_4bpp(2, 29); // BG2 copyright/press start
    PC_RenderBG_4bpp(1, 30); // BG1 box art mon (Charizard)
    PC_RenderBG_8bpp(0, 31); // BG0 game title logo (8bpp, highest priority)
    PC_RenderSprites();       // Sprites on top
}

#endif // USE_OPENGL
#endif // PLATFORM_PC
