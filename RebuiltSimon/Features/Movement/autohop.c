#include "RebuiltSimon/globals.h"
#include "RebuiltSimon/cvars.h"
#include "RebuiltSimon/SDK/GoldSrc/input.h"

void autohop(usercmd_t* cmd) {
    if (g_CoF.gclmove->movetype == MOVETYPE_WALK && CVAR_ON(autojump) && !(CVAR_ON(ducktap_priority) && in_ducktap.state & 1)) {
        if (!(g_CoF.gclmove->flags & FL_ONGROUND) && g_CoF.gclmove->waterlevel <= 1) {
            cmd->buttons &= ~IN_JUMP;
        }
    }
}