#ifdef PLATFORM_PC

#include "global.h"
#include "sprite.h"
#include "decompress.h"
#include <string.h>
#include <SDL2/SDL.h>

// OBJ VRAM starts at 0x10000 in our simulated VRAM
#define OBJ_VRAM_OFFSET 0x10000
#define OBJ_VRAM_SIZE   0x8000
#define MAX_OBJ_TILES   1024

// Tile allocation bitmap - tracks which 32-byte tile slots are used
static u32 sTileAllocBitmap[MAX_OBJ_TILES / 32] = {0};

// Sprite palette slots (16 palettes x 16 colors, after BG palettes)
// OBJ palettes start at palette slot 16 in gPlttBufferFaded
#define OBJ_PAL_OFFSET 256  // 16 BG palettes x 16 colors

// Tag lookup tables
#define MAX_TAGS 64
static u16 sSpriteTileTags[MAX_TAGS];
static u16 sSpriteTileStarts[MAX_TAGS];
static u16 sSpritePalTags[16];
static u8  sSpritePalNums[16];
static u8  sNumTileTags = 0;
static u8  sNumPalTags = 0;

extern u8 gVRAM[];
extern u16 gPlttBufferFaded[];
extern u16 gPlttBufferUnfaded[];

// The actual sprite objects
struct Sprite gSprites[MAX_SPRITES];
static u8 sNumSprites = 0;

// OAM double buffer
struct OamData gOamBuffer[128];

void ResetSpriteData(void)
{
    memset(gSprites, 0, sizeof(gSprites));
    memset(gOamBuffer, 0, sizeof(gOamBuffer));
    memset(sTileAllocBitmap, 0, sizeof(sTileAllocBitmap));
    memset(sSpriteTileTags, 0xFF, sizeof(sSpriteTileTags));
    memset(sSpritePalTags, 0xFF, sizeof(sSpritePalTags));
    sNumTileTags = 0;
    sNumPalTags = 0;
    sNumSprites = 0;
    for (int i = 0; i < MAX_SPRITES; i++)
    {
        gSprites[i].inUse = FALSE;
        gSprites[i].oam.y = 160; // offscreen
    }
}

s16 AllocSpriteTiles(u16 tileCount)
{
    // Find a contiguous run of free tiles
    for (int i = 0; i <= MAX_OBJ_TILES - tileCount; i++)
    {
        int found = 1;
        for (int j = 0; j < tileCount; j++)
        {
            int bit = i + j;
            if (sTileAllocBitmap[bit / 32] & (1 << (bit % 32)))
            {
                found = 0;
                break;
            }
        }
        if (found)
        {
            for (int j = 0; j < tileCount; j++)
            {
                int bit = i + j;
                sTileAllocBitmap[bit / 32] |= (1 << (bit % 32));
            }
            return i;
        }
    }
    return -1;
}

u16 LoadSpriteSheet(const struct SpriteSheet *sheet)
{
    u16 tileCount = sheet->size / 32; // 32 bytes per 4bpp 8x8 tile
    s16 tileStart = AllocSpriteTiles(tileCount);
    if (tileStart < 0)
    {
        SDL_Log("LoadSpriteSheet: out of OBJ VRAM tiles!");
        return 0xFFFF;
    }

    // Copy tile data to OBJ VRAM
    u8 *dest = gVRAM + OBJ_VRAM_OFFSET + (tileStart * 32);
    memcpy(dest, sheet->data, sheet->size);

    // Register tag
    if (sNumTileTags < MAX_TAGS)
    {
        sSpriteTileTags[sNumTileTags] = sheet->tag;
        sSpriteTileStarts[sNumTileTags] = tileStart;
        sNumTileTags++;
    }

    return tileStart;
}

void LoadSpriteSheets(const struct SpriteSheet *sheets)
{
    for (int i = 0; sheets[i].data != NULL; i++)
        LoadSpriteSheet(&sheets[i]);
}

u16 GetSpriteTileStartByTag(u16 tag)
{
    for (int i = 0; i < sNumTileTags; i++)
        if (sSpriteTileTags[i] == tag)
            return sSpriteTileStarts[i];
    return 0xFFFF;
}

u8 LoadSpritePalette(const struct SpritePalette *palette)
{
    // Find a free OBJ palette slot
    for (int i = 0; i < 16; i++)
    {
        if (sSpritePalTags[i] == 0xFFFF)
        {
            sSpritePalTags[i] = palette->tag;
            sSpritePalNums[i] = i;
            u16 *dest = gPlttBufferFaded + OBJ_PAL_OFFSET + (i * 16);
            u16 *dest2 = gPlttBufferUnfaded + OBJ_PAL_OFFSET + (i * 16);
            memcpy(dest, palette->data, 32);
            memcpy(dest2, palette->data, 32);
            return i;
        }
    }
    return 0xFF;
}

void LoadSpritePalettes(const struct SpritePalette *palettes)
{
    for (int i = 0; palettes[i].data != NULL; i++)
        LoadSpritePalette(&palettes[i]);
}

u8 IndexOfSpritePaletteTag(u16 tag)
{
    for (int i = 0; i < 16; i++)
        if (sSpritePalTags[i] == tag)
            return i;
    return 0xFF;
}

void FreeAllSpritePalettes(void)
{
    memset(sSpritePalTags, 0xFF, sizeof(sSpritePalTags));
    sNumPalTags = 0;
}

void FreeSpriteTilesByTag(u16 tag)
{
    for (int i = 0; i < sNumTileTags; i++)
    {
        if (sSpriteTileTags[i] == tag)
        {
            sSpriteTileTags[i] = 0xFFFF;
            // Could free bitmap here but skip for now
            return;
        }
    }
}

u8 CreateSprite(const struct SpriteTemplate *template, s16 x, s16 y, u8 subpriority)
{
    for (int i = 0; i < MAX_SPRITES; i++)
    {
        if (!gSprites[i].inUse)
        {
            struct Sprite *sprite = &gSprites[i];
            memset(sprite, 0, sizeof(*sprite));
            sprite->inUse = TRUE;
            sprite->x = x;
            sprite->y = y;
            sprite->subpriority = subpriority;
            sprite->oam = *template->oam;
            sprite->anims = template->anims;
            sprite->images = template->images;
            sprite->affineAnims = template->affineAnims;
            sprite->callback = template->callback;

            // Look up tile start from tag
            u16 tileStart = GetSpriteTileStartByTag(template->tileTag);
            if (tileStart != 0xFFFF)
                sprite->oam.tileNum = tileStart;

            // Look up palette
            u8 palNum = IndexOfSpritePaletteTag(template->paletteTag);
            if (palNum != 0xFF)
                sprite->oam.paletteNum = palNum;

            sprite->oam.x = x;
            sprite->oam.y = y;

            sNumSprites++;
            return i;
        }
    }
    return MAX_SPRITES; // no free slot
}

u8 CreateSpriteAtEnd(const struct SpriteTemplate *template, s16 x, s16 y, u8 subpriority)
{
    return CreateSprite(template, x, y, subpriority);
}

u8 CreateSpriteAndAnimate(const struct SpriteTemplate *template, s16 x, s16 y, u8 subpriority)
{
    return CreateSprite(template, x, y, subpriority);
}

u8 CreateInvisibleSprite(void (*callback)(struct Sprite *))
{
    return MAX_SPRITES;
}

void DestroySprite(struct Sprite *sprite)
{
    sprite->inUse = FALSE;
    sprite->oam.y = 160;
}

void AnimateSprites(void)
{
    for (int i = 0; i < MAX_SPRITES; i++)
    {
        if (gSprites[i].inUse && gSprites[i].callback)
            gSprites[i].callback(&gSprites[i]);
    }
}

void BuildOamBuffer(void)
{
    int oamIdx = 0;
    for (int i = 0; i < MAX_SPRITES && oamIdx < 128; i++)
    {
        if (gSprites[i].inUse && !gSprites[i].invisible)
        {
            gOamBuffer[oamIdx] = gSprites[i].oam;
            gOamBuffer[oamIdx].x = gSprites[i].x;
            gOamBuffer[oamIdx].y = gSprites[i].y;
            oamIdx++;
        }
    }
    // Hide remaining OAM entries
    for (; oamIdx < 128; oamIdx++)
        gOamBuffer[oamIdx].y = 160;
}

void ClearSpriteCopyRequests(void) {}
void ResetAffineAnimData(void) {}
void ResetOamRange(u8 a, u8 b) {}
void StartSpriteAnim(struct Sprite *sprite, u8 animNum) { sprite->animNum = animNum; sprite->animEnded = FALSE; }
void StartSpriteAnimIfDifferent(struct Sprite *sprite, u8 animNum) { if (sprite->animNum != animNum) StartSpriteAnim(sprite, animNum); }
void SeekSpriteAnim(struct Sprite *sprite, u8 animCmdIndex) {}
void StartSpriteAffineAnim(struct Sprite *sprite, u8 animNum) {}
void StartSpriteAffineAnimIfDifferent(struct Sprite *sprite, u8 animNum) {}
void ChangeSpriteAffineAnim(struct Sprite *sprite, u8 animNum) {}
void ChangeSpriteAffineAnimIfDifferent(struct Sprite *sprite, u8 animNum) {}
void SetSpriteSheetFrameTileNum(struct Sprite *sprite) {}
u8 AllocOamMatrix(void) { return 0; }
void FreeOamMatrix(u8 matrixNum) {}
void InitSpriteAffineAnim(struct Sprite *sprite) {}
void SetOamMatrixRotationScaling(u8 matrixNum, s16 xScale, s16 yScale, u16 rotation) {}
void AllocTilesForSpriteSheets(struct SpriteSheet *sheets) {}
u16 AllocTilesForSpriteSheet(struct SpriteSheet *sheet) { return 0; }
void FreeSpriteTileRanges(void) {}
u16 GetSpriteTileTagByTileStart(u16 start) { return 0xFFFF; }
void RequestSpriteSheetCopy(const struct SpriteSheet *sheet) {}
u16 LoadSpriteSheetDeferred(const struct SpriteSheet *sheet) { return LoadSpriteSheet(sheet); }
u16 GetSpritePaletteTagByPaletteNum(u8 paletteNum) { return 0xFFFF; }
void FreeSpritePaletteByTag(u16 tag) {}
void SetSubspriteTables(struct Sprite *sprite, const struct SubspriteTable *subspriteTables) {}
bool8 AddSpriteToOamBuffer(struct Sprite *object, u8 *oamIndex) { return FALSE; }
bool8 AddSubspritesToOamBuffer(struct Sprite *sprite, struct OamData *destOam, u8 *oamIndex) { return FALSE; }
void CopyToSprites(u8 *src) {}
void CopyFromSprites(u8 *dest) {}
u8 SpriteTileAllocBitmapOp(u16 bit, u8 op) { return 0; }
void FreeSpriteTilesIfNotUsingSheet(struct Sprite *sprite) {}
void SetSpriteMatrixAnchor(struct Sprite *sprite, s16 xmod, s16 ymod) {}
u8 AllocSpritePalette(u16 tag) { return LoadSpritePalette((struct SpritePalette *)&tag); }

#endif // PLATFORM_PC
