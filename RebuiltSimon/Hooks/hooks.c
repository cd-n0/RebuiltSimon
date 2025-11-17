#include "RebuiltSimon/globals.h"
#include "hooks.h"

bool initialize_hooks(void) {
    MH_Initialize();

    for (size_t i = 0; i < NUM_HOOKS; ++i) {
        hook_inits[i]();
    }

    if (MH_OK != MH_EnableHook(MH_ALL_HOOKS)) {
        g_CoF.pEngine->Con_Printf("Failed hooking functions\n");
        return false;
    }

    return true;
}

void deinitialize_hooks(void) {
    MH_Uninitialize();
}