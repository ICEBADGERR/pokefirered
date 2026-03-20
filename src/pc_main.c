#ifdef PLATFORM_PC

#include <SDL2/SDL.h>
#include "pc_port.h"
#include "main.h"
#include "malloc.h"

// -------------------------------------------------------
// GBA hardware register definitions (stubs)
// -------------------------------------------------------
u16 PC_REG_DISPCNT   = 0;
u16 PC_REG_DISPSTAT  = 0;
u16 PC_REG_VCOUNT    = 0;
u16 PC_REG_WAITCNT   = 0;
u16 PC_REG_KEYINPUT  = 0x03FF; // all buttons released (GBA active-low)
u16 PC_REG_IME       = 0;
u16 PC_REG_IE        = 0;
u16 PC_REG_IF        = 0;
u16 PC_REG_TM1CNT_L  = 0;
u16 PC_REG_TM1CNT_H  = 0;
u32 PC_INTR_CHECK    = 0;
u32 PC_INTR_VECTOR   = 0;
u16 PC_BG_PLTT[256]  = {0};

#define GBA_WIDTH  240
#define GBA_HEIGHT 160
#define SCALE      3

SDL_Window   *gWindow   = NULL;
SDL_Renderer *gRenderer = NULL;
SDL_Texture  *gTexture  = NULL;

u16 gFramebuffer[GBA_WIDTH * GBA_HEIGHT];

// Declared in pc_game.c
void InitMainCallbacks(void);

void PC_CallCallbacks(void);
void PC_RunFrame(void);

static u32 RGB555toARGB(u16 color)
{
    u8 r = (color >>  0) & 0x1F;
    u8 g = (color >>  5) & 0x1F;
    u8 b = (color >> 10) & 0x1F;
    r = (r << 3) | (r >> 2);
    g = (g << 3) | (g >> 2);
    b = (b << 3) | (b >> 2);
    return (0xFF << 24) | (r << 16) | (g << 8) | b;
}

static void RenderFramebuffer(void)
{
    u32 pixels[GBA_WIDTH * GBA_HEIGHT];
    for (int i = 0; i < GBA_WIDTH * GBA_HEIGHT; i++)
        pixels[i] = RGB555toARGB(gFramebuffer[i]);

    SDL_UpdateTexture(gTexture, NULL, pixels, GBA_WIDTH * sizeof(u32));
    SDL_RenderClear(gRenderer);
    SDL_RenderCopy(gRenderer, gTexture, NULL, NULL);
    SDL_RenderPresent(gRenderer);
}

int main(int argc, char *argv[])
{
    SDL_Init(SDL_INIT_VIDEO);

    gWindow = SDL_CreateWindow(
        "Pokemon FireRed PC",
        SDL_WINDOWPOS_CENTERED,
        SDL_WINDOWPOS_CENTERED,
        GBA_WIDTH * SCALE, GBA_HEIGHT * SCALE,
        SDL_WINDOW_SHOWN | SDL_WINDOW_RESIZABLE
    );

    gRenderer = SDL_CreateRenderer(gWindow, -1, SDL_RENDERER_ACCELERATED);
    SDL_RenderSetLogicalSize(gRenderer, GBA_WIDTH, GBA_HEIGHT);

    gTexture = SDL_CreateTexture(
        gRenderer,
        SDL_PIXELFORMAT_ARGB8888,
        SDL_TEXTUREACCESS_STREAMING,
        GBA_WIDTH, GBA_HEIGHT
    );

    // Initialize the game callback system
    InitHeap(gHeap, HEAP_SIZE);
    InitMainCallbacks();
    

    SDL_Event event;
    int running = 1;

    while (running)
    {
        while (SDL_PollEvent(&event))
            if (event.type == SDL_QUIT)
                running = 0;

        // Tick the game state machine
        PC_RunFrame();
        PC_CallCallbacks();

        RenderFramebuffer();
        SDL_Delay(16);
    }

    SDL_DestroyTexture(gTexture);
    SDL_DestroyRenderer(gRenderer);
    SDL_DestroyWindow(gWindow);
    SDL_Quit();
    return 0;
}

#endif // PLATFORM_PC
