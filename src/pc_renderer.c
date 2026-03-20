#ifdef PLATFORM_PC

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
    u16 *tileMap  = (u16 *)(gVRAM + 0x10000 + (screenBase * 0x800));

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
    u16 *tileMap  = (u16 *)(gVRAM + 0x10000 + (screenBase * 0x800));

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

void PC_RenderFrame(void)
{
    // Clear to backdrop color (palette[0])
    u16 backdrop = gPlttBufferFaded[0];
    for (int i = 0; i < GBA_WIDTH * GBA_HEIGHT; i++)
        gFramebuffer[i] = backdrop;

    // Render BG layers back to front (lower priority first)
    PC_RenderBG_4bpp(3, 3); // BG3 border (lowest)
    PC_RenderBG_4bpp(2, 2); // BG2 copyright/press start
    PC_RenderBG_4bpp(1, 1); // BG1 box art mon (Charizard)
    PC_RenderBG_8bpp(0, 0); // BG0 game title logo (highest, 8bpp)
}

#endif // PLATFORM_PC
