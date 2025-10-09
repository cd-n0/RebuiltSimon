#include "RebuiltSimon/globals.h"
#include "RebuiltSimon/Features/Visual/info.h"

typedef __int32(__cdecl* HUD_Frame_t)(double time);
static HUD_Frame_t HUD_Frame_o;

static void HUD_Frame_h(double time) {
    HUD_Frame_o(time);
    rebuilt_simon_info_frame(time);
}

void hook_HUD_Frame() {
    CreateHook(g_CoF.pClient->pfnFrame, HUD_Frame_h, &HUD_Frame_o);
}