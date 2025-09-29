#include "RebuiltSimon/globals.h"
#include "RebuiltSimon/cvars.h"

static const char* PATTERN_PM_PreventMegaBunnyJumping = "55 8B EC 83 EC ? A1 ? ? ? ? 0F 57 C0";

typedef void(__cdecl* PATTERN_PM_PreventMegaBunnyJumping_t)(void);
static PATTERN_PM_PreventMegaBunnyJumping_t PATTERN_PM_PreventMegaBunnyJumping_o;


static void __cdecl PATTERN_PM_PreventMegaBunnyJumping_h(void) {
    if (CVAR_ON(bhop_cap))
        PATTERN_PM_PreventMegaBunnyJumping_o();
};

void hook_PM_PreventMegaBunnyJumping() {
    PATTERN_PM_PreventMegaBunnyJumping_o = (PATTERN_PM_PreventMegaBunnyJumping_t)cdmem_find_signature("hl.dll", PATTERN_PM_PreventMegaBunnyJumping); // TODO: Write a better signature scanner
    if (PATTERN_PM_PreventMegaBunnyJumping_o) {
        CreateHook(PATTERN_PM_PreventMegaBunnyJumping_o, PATTERN_PM_PreventMegaBunnyJumping_h, &PATTERN_PM_PreventMegaBunnyJumping_o);
    }
    else {
        g_CoF.pEngine->Con_Printf("Failed to find bhop prevention function signature bhop_cap cvar will not work\n");
    }
}