#include "RebuiltSimon/globals.h"
#include "RebuiltSimon/Hooks/hooks.h"

typedef void(__cdecl* HUD_Shutdown_t)(void);
static HUD_Shutdown_t HUD_Shutdown_o;

static void HUD_Shutdown_h(void) {
    HUD_Shutdown_o();
    deinitialize_hooks();
}

void hook_HUD_Shutdown() {
    CreateHook(g_CoF.pClient->pfnShutdown, HUD_Shutdown_h, &HUD_Shutdown_o);
}