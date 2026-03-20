#ifdef PLATFORM_PC

#include "global.h"
#include "m4a.h"

// GBA audio engine stubs - will be replaced with SDL_mixer later
struct MusicPlayerInfo gMPlayInfo_BGM = {0};
struct MusicPlayerInfo gMPlayInfo_SE1 = {0};
struct MusicPlayerInfo gMPlayInfo_SE2 = {0};
struct MusicPlayerInfo gMPlayInfo_SE3 = {0};
struct SoundInfo gSoundInfo = {0};

void m4aSoundVSync(void) {}
void m4aSoundVSyncOn(void) {}
void m4aSoundInit(void) {}
void m4aSoundMain(void) {}
void m4aSongNumStart(u16 n) {}
void m4aSongNumStartOrChange(u16 n) {}
void m4aSongNumStop(u16 n) {}
void m4aMPlayAllStop(void) {}
void m4aMPlayContinue(struct MusicPlayerInfo *mplayInfo) {}
void m4aMPlayFadeOut(struct MusicPlayerInfo *mplayInfo, u16 speed) {}
void m4aMPlayFadeOutTemporarily(struct MusicPlayerInfo *mplayInfo, u16 speed) {}
void m4aMPlayFadeIn(struct MusicPlayerInfo *mplayInfo, u16 speed) {}
void m4aMPlayImmInit(struct MusicPlayerInfo *mplayInfo) {}

#endif // PLATFORM_PC
void SetPokemonCryStereo(u32 val) {}
