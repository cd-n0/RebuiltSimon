#include "RebuiltSimon/globals.h"
#include "RebuiltSimon/cvars.h"
#include "RebuiltSimon/SDK/Helpers/Math/vector_math.h"

bool in_game() {
    if (g_CoF.pClientState->state != ca_active) return false;
    const char* level_name = g_CoF.pEngine->pfnGetLevelName();
    /* Are we on the 3D menu? */
    if (!level_name || !strncmp(level_name, "maps/c_game_menu", 16)) return false;
    return true;
}

/* I know this is a really stupid and not guaranteed way to do it but it works idc */
int get_stamina() {
    static int* stamina = NULL;
    if (stamina) {
        return *stamina;
    }
    /* No idea what this structure is reverse more */
    int **unk = OFFSET(g_hw_base, 0x7E0544);
    if (unk) {
        if (100 == *(int*)OFFSET(*unk, 0x1000)) {
            stamina = (int*)OFFSET(*unk, 0x1000);
        }
    }
    return 0;
}

int get_health() {
    int *hp = OFFSET(g_hw_base, 0x11115AC);
    if (hp) {
        return *hp;
    }
    else 0;
}

int get_speed() {
    if (CVAR_ON(absolute_speed)) {
        return vec3_len(g_CoF.gclmove->velocity);
    }
    else {
        return vec2_len(g_CoF.gclmove->velocity);
    }
}


int get_speed_vec3(vec3_t v) {
    if (CVAR_ON(absolute_speed)) {
        return vec3_len(v);
    }
    else {
        return vec2_len(v);
    }
}