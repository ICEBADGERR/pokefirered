#ifdef PLATFORM_PC

#include "global.h"
#include "save.h"

// Save status globals
u16 gSaveFileStatus = SAVE_STATUS_EMPTY;

void ClearSaveData(void) {}
void Save_ResetSaveCounters(void) {}
void Task_LinkFullSave(u8 taskId) {}

u8 HandleSavingData(u8 saveType) { return SAVE_STATUS_OK; }
u8 TrySavingData(u8 saveType) { return SAVE_STATUS_OK; }
u8 LoadGameSave(u8 saveType) { return SAVE_STATUS_EMPTY; }

bool8 LinkFullSave_Init(void) { return TRUE; }
bool8 LinkFullSave_WriteSector(void) { return TRUE; }
bool8 LinkFullSave_ReplaceLastSector(void) { return TRUE; }
bool8 LinkFullSave_SetLastSectorSignature(void) { return TRUE; }
bool8 WriteSaveBlock2(void) { return TRUE; }
bool8 WriteSaveBlock1Sector(void) { return TRUE; }

u32 TryReadSpecialSaveSector(u8 sector, u8 *dst) { return 0; }
u32 TryWriteSpecialSaveSector(u8 sector, u8 *src) { return 0; }

#endif // PLATFORM_PC
