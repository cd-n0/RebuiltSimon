#include "RebuiltSimon/globals.h"
#include "RebuiltSimon/SDK/Helpers/SpriteUtils/sprite_utils.h"
#include "RebuiltSimon/SDK/Helpers/Parsers/engine_font.h"

typedef __int32(__cdecl* HUD_VidInit_t)(void);
static HUD_VidInit_t HUD_VidInit_o;

static void HUD_VidInit_h(void) {
    HUD_VidInit_o();
    screen_info.iSize = sizeof(screen_info); /* This took 3 days to find out */
    g_CoF.pEngine->pfnGetScreenInfo(&screen_info);
    initialize_sprites();

    int height = get_engine_font_height(screen_info.iHeight);
}

void hook_HUD_VidInit() {
    CreateHook(g_CoF.pClient->pfnVidInit, HUD_VidInit_h, &HUD_VidInit_o);
}