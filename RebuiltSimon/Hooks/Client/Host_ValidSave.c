#include "RebuiltSimon/cvars.h"
#include "RebuiltSimon/globals.h"

/* ty for the save point string Andreas */
static const char* PATTERN_Host_ValidSave = "55 8B EC 83 3D ? ? ? ? ? 74 ? 33 C0 E9";

typedef __int32(__cdecl* prevent_save_func_t)(void);
static prevent_save_func_t Host_ValidSave_o;

static __int32 __cdecl Host_ValidSave_h(void) {
    if (CVAR_ON(cof_save_prevention))
        return Host_ValidSave_o();
    return 1;
};

void hook_Host_ValidSave() {
    if (GetModuleHandleA("BunnymodXT.dll")) {
        LOG_INFO("BunnymodXT is present, skipping hooking Host_ValidSave to prevent crash caused by BunnymodXT");

        /* Due to how BXT straight up crash the game if it can't find the Host_ValidSave signature
           therefore not setting the is cof steam flag, and due to how function hooking and signature
           scanning works these 2 mods are incompatible. */
        LOG_WARN("RebuiltSimon and BXT is not compatible, some functionality may not work");

        return;
    }

    Host_ValidSave_o = (prevent_save_func_t)cdmem_find_signature("hw.dll", PATTERN_Host_ValidSave);
    if (Host_ValidSave_o) {
        CreateHook(Host_ValidSave_o, Host_ValidSave_h, &Host_ValidSave_o);
    }
    else {
        LOG_WARN("Failed to find save prevention signature cof_save_prevention cvar will not work");
    }
}