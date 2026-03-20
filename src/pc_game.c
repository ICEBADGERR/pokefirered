#ifdef PLATFORM_PC

#include "global.h"
#include "main.h"
#include "malloc.h"
#include "task.h"
#include "intro.h"

MainCallback gallback1 = NULL;
MainCallback gallback2 = NULL;
struct Main gMain = {0};
struct SaveBlock2 *gSaveBlock2Ptr = NULL;
struct SaveBlock1 *gSaveBlock1Ptr = NULL;
u8 gHeap[HEAP_SIZE];

void SetMainCallback2(MainCallback callback)
{
    gallback2 = callback;
    gMain.state = 0;
}

void InitMainCallbacks(void)
{
    gMain.vblankCounter1 = 0;
    gMain.vblankCounter2 = 0;
    gallback1 = NULL;
    ResetTasks();
    SetMainCallback2(CB2_InitCopyrightScreenAfterBootup);
}

void PC_RunFrame(void)
{
    RunTasks();
}

void PC_CallCallbacks(void)
{
    if (gallback1)
        gallback1();
    if (gallback2)
        gallback2();
}

#endif // PLATFORM_PC
