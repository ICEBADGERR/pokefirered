#ifdef PLATFORM_PC

#include "global.h"
#include "sound.h"

void InitMapMusic(void) {}
void MapMusicMain(void) {}
void ResetMapMusic(void) {}
void PlayNewMapMusic(u16 songNum) {}
void StopMapMusic(void) {}
void FadeOutMapMusic(u8 speed) {}
void FadeOutAndPlayNewMapMusic(u16 songNum, u8 speed) {}
void FadeOutAndFadeInNewMapMusic(u16 songNum, u8 fadeOutSpeed, u8 fadeInSpeed) {}
void PlayFanfareByFanfareNum(u8 fanfareNum) {}
void StopFanfareByFanfareNum(u8 fanfareNum) {}
void PlayFanfare(u16 songNum) {}
void FadeInNewBGM(u16 songNum, u8 speed) {}
void FadeOutBGMTemporarily(u8 speed) {}
void FadeInBGM(u8 speed) {}
void FadeOutBGM(u8 speed) {}
void PlaySE(u16 songNum) {}
void PlaySE2(u16 songNum) {}
void PlaySE3(u16 songNum) {}
void StopSE(void) {}
u16 GetCurrentMapMusic(void) { return 0; }
bool8 IsNotWaitingForBGMStop(void) { return TRUE; }
bool8 WaitFanfare(bool8 stop) { return TRUE; }
bool8 IsFanfareTaskInactive(void) { return TRUE; }
bool8 IsBGMPausedOrStopped(void) { return TRUE; }

#endif // PLATFORM_PC
