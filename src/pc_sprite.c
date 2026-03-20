#ifdef PLATFORM_PC

#include "global.h"
#include "sprite.h"

void ResetSpriteData(void) {}
void AnimateSprites(void) {}
void BuildOamBuffer(void) {}
void DestroySprite(struct Sprite *sprite) {}
void ResetOamRange(u8 a, u8 b) {}
void StartSpriteAnim(struct Sprite *sprite, u8 animNum) {}
void StartSpriteAnimIfDifferent(struct Sprite *sprite, u8 animNum) {}
void SeekSpriteAnim(struct Sprite *sprite, u8 animCmdIndex) {}
void StartSpriteAffineAnim(struct Sprite *sprite, u8 animNum) {}
void StartSpriteAffineAnimIfDifferent(struct Sprite *sprite, u8 animNum) {}
void ChangeSpriteAffineAnim(struct Sprite *sprite, u8 animNum) {}
void ChangeSpriteAffineAnimIfDifferent(struct Sprite *sprite, u8 animNum) {}
void SetSpriteSheetFrameTileNum(struct Sprite *sprite) {}
void FreeOamMatrix(u8 matrixNum) {}
void InitSpriteAffineAnim(struct Sprite *sprite) {}
void SetOamMatrixRotationScaling(u8 matrixNum, s16 xScale, s16 yScale, u16 rotation) {}
void LoadSpriteSheets(const struct SpriteSheet *sheets) {}
void AllocTilesForSpriteSheets(struct SpriteSheet *sheets) {}
void FreeSpriteTilesByTag(u16 tag) {}
void FreeSpriteTileRanges(void) {}
void RequestSpriteSheetCopy(const struct SpriteSheet *sheet) {}
void FreeAllSpritePalettes(void) {}
void LoadSpritePalettes(const struct SpritePalette *palettes) {}
void FreeSpritePaletteByTag(u16 tag) {}
void SetSubspriteTables(struct Sprite *sprite, const struct SubspriteTable *subspriteTables) {}
void CopyToSprites(u8 *src) {}
void CopyFromSprites(u8 *dest) {}
void ClearSpriteCopyRequests(void) {}
void ResetAffineAnimData(void) {}
void FreeSpriteTilesIfNotUsingSheet(struct Sprite *sprite) {}
void SetSpriteMatrixAnchor(struct Sprite *sprite, s16 xmod, s16 ymod) {}

u8 CreateSprite(const struct SpriteTemplate *template, s16 x, s16 y, u8 subpriority) { return 0; }
u8 CreateSpriteAtEnd(const struct SpriteTemplate *template, s16 x, s16 y, u8 subpriority) { return 0; }
u8 CreateInvisibleSprite(void (*callback)(struct Sprite *)) { return 0; }
u8 CreateSpriteAndAnimate(const struct SpriteTemplate *template, s16 x, s16 y, u8 subpriority) { return 0; }
u8 AllocOamMatrix(void) { return 0; }
u8 LoadSpritePalette(const struct SpritePalette *palette) { return 0; }
u8 AllocSpritePalette(u16 tag) { return 0; }
u8 IndexOfSpritePaletteTag(u16 tag) { return 0xFF; }
u8 SpriteTileAllocBitmapOp(u16 bit, u8 op) { return 0; }

u16 LoadSpriteSheet(const struct SpriteSheet *sheet) { return 0; }
u16 AllocTilesForSpriteSheet(struct SpriteSheet *sheet) { return 0; }
u16 GetSpriteTileStartByTag(u16 tag) { return 0xFFFF; }
u16 GetSpriteTileTagByTileStart(u16 start) { return 0xFFFF; }
u16 LoadSpriteSheetDeferred(const struct SpriteSheet *sheet) { return 0; }
u16 GetSpritePaletteTagByPaletteNum(u8 paletteNum) { return 0xFFFF; }

s16 AllocSpriteTiles(u16 tileCount) { return 0; }

bool8 AddSpriteToOamBuffer(struct Sprite *object, u8 *oamIndex) { return FALSE; }
bool8 AddSubspritesToOamBuffer(struct Sprite *sprite, struct OamData *destOam, u8 *oamIndex) { return FALSE; }

#endif // PLATFORM_PC
