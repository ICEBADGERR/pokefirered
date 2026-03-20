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

// Render BG layer 0 (4bpp mode) to gFramebuffer
// charBase: tile graphics base in VRAM (in units of 16KB)
// screenBase: tilemap base in VRAM (in units of 2KB)
void PC_RenderBG0(u8 charBase, u8 screenBase)
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

            // 4bpp: each tile is 32 bytes (8x8 pixels, 4 bits each)
            u8 *tile = tileData + (tileIndex * 32);
            u16 *pal = gPlttBufferFaded + (paletteNum * 16);

            for (int py = 0; py < TILE_SIZE; py++)
            {
                for (int px = 0; px < TILE_SIZE; px++)
                {
                    // Each byte holds 2 pixels
                    u8 byte = tile[(py * 4) + (px / 2)];
                    u8 colorIndex = (px & 1) ? (byte >> 4) : (byte & 0xF);

                    int screenX = tx * TILE_SIZE + px;
                    int screenY = ty * TILE_SIZE + py;

                    // Color 0 is transparent — use backdrop color
                    if (colorIndex == 0)
                        gFramebuffer[screenY * GBA_WIDTH + screenX] = gPlttBufferFaded[0];
                    else
                        gFramebuffer[screenY * GBA_WIDTH + screenX] = pal[colorIndex];
                }
            }
        }
    }
}

// Main render function called each frame
void PC_RenderFrame(void)
{
    // For now render BG0 with charBase=0, screenBase=7
    // (these are what the copyright screen uses)
    PC_RenderBG0(0, 7);
}

#endif // PLATFORM_PC
