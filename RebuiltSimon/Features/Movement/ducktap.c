#include "RebuiltSimon/globals.h"
#include "RebuiltSimon/SDK/GoldSrc/input.h"

void ducktap(usercmd_t* cmd) {
    if (in_ducktap.state & 1) {
        static bool was_ducking = false;
        if (g_CoF.gclmove->movetype == MOVETYPE_WALK && g_CoF.gclmove->flags & FL_ONGROUND && was_ducking) {
            cmd->buttons |= IN_DUCK;
            was_ducking = false;
        }
        else {
            cmd->buttons &= ~IN_DUCK;
            was_ducking = true;
        }
    }
}