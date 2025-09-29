#include "RebuiltSimon/globals.h"
#include "hooks.h"

//static const char* PM_JUMP_PATTERN = "55 8B EC 83 E4 ? 8B 0D";

/* This function seems to only work at the engine initialization and looks hunk alloced, write a way to add commands yourself */
/* static const char* CMD_ADD_COMMAND_PATTERN = "55 8B EC 83 EC ? 83 3D ? ? ? ? ? 74 ? 68 ? ? ? ? E8 ? ? ? ? 83 C4 ? 8B 45"; */

//typedef void(__cdecl* pm_jump_func_t)(void);
//static pm_jump_func_t pm_jump_o;

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