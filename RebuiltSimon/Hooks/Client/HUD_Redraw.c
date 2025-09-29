#include "RebuiltSimon/globals.h"
#include "RebuiltSimon/Features/hud.h"

typedef __int32(__cdecl* HUD_Redraw_t)(float time, int intermission);
static HUD_Redraw_t HUD_Redraw_o;

static int HUD_Redraw_h(float time, int intermission) {
    rebuilt_simon_hud_frame(time);  /* This took a week to find out and I wouldn't have if it wasn't for bxt */
    return HUD_Redraw_o(time, intermission);
}

void hook_HUD_Redraw() {
    CreateHook(g_CoF.pClient->pfnRedraw, HUD_Redraw_h, &HUD_Redraw_o);
}