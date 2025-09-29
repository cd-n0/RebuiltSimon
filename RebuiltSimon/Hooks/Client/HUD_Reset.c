#include "RebuiltSimon/globals.h"
#include "RebuiltSimon/SDK/Helpers/SpriteUtils/sprite_utils.h"

typedef void(__cdecl* HUD_Reset_t)(void);
static HUD_Reset_t HUD_Reset_o;

static void HUD_Reset_h(void) {
    HUD_Reset_o();
    g_CoF.pEngine->pfnGetScreenInfo(&screen_info);
    initialize_sprites();
}

void hook_HUD_Reset() {
    CreateHook(g_CoF.pClient->pfnReset, HUD_Reset_h, &HUD_Reset_o);
}