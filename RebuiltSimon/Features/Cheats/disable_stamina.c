#include "RebuiltSimon/globals.h"
#include "RebuiltSimon/cvars.h"

void disable_stamina() {
    if (g_CoF.gsv_player) {
        edict_t** dict = g_CoF.gsv_player;
        if (dict) {
            simon_t* simon = (simon_t*)(*dict)->pvPrivateData;
            if (simon) { simon->infinite_stamina = CVAR_ON(disable_stamina) ? 1 : 0; }
        }
    };
}
