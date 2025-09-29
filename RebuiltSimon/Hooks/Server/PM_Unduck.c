#include "RebuiltSimon/globals.h"
#include "RebuiltSimon/cvars.h"

static const char* PATTERN_PM_Unduck = "53 8B DC 83 EC ? 83 E4 ? 83 C4 ? 55 8B 6B ? 89 6C 24 ? 8B EC 81 EC ? ? ? ? A1 ? ? ? ? 33 C5 89 45 ? A1";

/* not actually cdecl but it seems to work fine?
int __usercall PM_Unduck@<eax>(int a1@<ebp>)
.
.
.
mov     esp, ebx
pop     ebx
retn
*/
typedef void(__cdecl* PM_Unduck_t)(void);
static PM_Unduck_t PM_Unduck_o;

static void __cdecl PM_Unduck_h(void) {
    if (CVAR_ON(disable_duck_time)) {
        g_CoF.pmove->flags |= FL_DUCKING;
        g_CoF.pmove->bInDuck = false;
    }
    PM_Unduck_o();
};

void hook_PM_Unduck() {
    PM_Unduck_o = (PM_Unduck_t)cdmem_find_signature("hl.dll", PATTERN_PM_Unduck);
    if (PM_Unduck_o) {
        CreateHook(PM_Unduck_o, PM_Unduck_h, &PM_Unduck_o);
    }
    else {
        g_CoF.pEngine->Con_Printf("Failed to find unduck function signature hl_unduck cvar will not work\n");
    }
}