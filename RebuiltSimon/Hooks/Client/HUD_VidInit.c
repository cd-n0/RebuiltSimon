#include <libresparser.h>
#include "RebuiltSimon/globals.h"
#include "RebuiltSimon/SDK/Helpers/SpriteUtils/sprite_utils.h"

typedef __int32(__cdecl* HUD_VidInit_t)(void);
static HUD_VidInit_t HUD_VidInit_o;

static void HUD_VidInit_h(void) {
    HUD_VidInit_o();
    screen_info.iSize = sizeof(screen_info); /* This took 3 days to find out */
    g_CoF.pEngine->pfnGetScreenInfo(&screen_info);
    initialize_sprites();
    engine_font = get_engine_font(screen_info.iHeight);
}

void hook_HUD_VidInit() {
    CreateHook(g_CoF.pClient->pfnVidInit, HUD_VidInit_h, &HUD_VidInit_o);
}