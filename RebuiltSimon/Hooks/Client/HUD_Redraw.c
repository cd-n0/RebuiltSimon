#include "RebuiltSimon/globals.h"
#include "RebuiltSimon/Features/Visual/hud.h"
#include "RebuiltSimon/Features/Visual/info.h"

typedef __int32(__cdecl* HUD_Redraw_t)(float time, int intermission);
static HUD_Redraw_t HUD_Redraw_o;

static int HUD_Redraw_h(float time, int intermission) {
    rebuilt_simon_hud_HUD_Redraw(time);
    rebuilt_simon_info_HUD_Redraw(time);
    return HUD_Redraw_o(time, intermission);
}

void hook_HUD_Redraw() {
    CreateHook(g_CoF.pClient->pfnRedraw, HUD_Redraw_h, &HUD_Redraw_o);
}
