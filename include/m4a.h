#ifndef GUARD_M4A_H
#define GUARD_M4A_H

#ifndef PLATFORM_PC
#include "gba/m4a_internal.h"
#else
// Minimal stubs for m4a structs on PC
struct MusicPlayerInfo { u8 dummy[128]; };
struct SoundInfo { u32 pcmDmaCounter; u8 dummy[256]; };
struct SongHeader { u8 dummy[32]; };
#endif

void m4aSoundVSync(void);
void m4aSoundVSyncOn(void);

void m4aSoundInit(void);
void m4aSoundMain(void);
void m4aSongNumStart(u16 n);
void m4aSongNumStartOrChange(u16 n);
void m4aSongNumStop(u16 n);
void m4aMPlayAllStop(void);
void m4aMPlayContinue(struct MusicPlayerInfo *mplayInfo);
void m4aMPlayFadeOut(struct MusicPlayerInfo *mplayInfo, u16 speed);
void m4aMPlayFadeOutTemporarily(struct MusicPlayerInfo *mplayInfo, u16 speed);
void m4aMPlayFadeIn(struct MusicPlayerInfo *mplayInfo, u16 speed);
void m4aMPlayImmInit(struct MusicPlayerInfo *mplayInfo);

extern struct MusicPlayerInfo gMPlayInfo_BGM;
extern struct MusicPlayerInfo gMPlayInfo_SE1;
extern struct MusicPlayerInfo gMPlayInfo_SE2;
extern struct MusicPlayerInfo gMPlayInfo_SE3;
extern struct SoundInfo gSoundInfo;

extern const struct SongHeader mus_victory_gym_leader;

#endif //GUARD_M4A_H
