#include "pch.h"
#include "SDK/structures.h"
#include "globals.h"

HMODULE g_hl_base;
HMODULE g_hw_base;
HMODULE g_client_base;
cof_data_t g_CoF;
SCREENINFO screen_info;
cl_entity_t* localplayer;
engine_font_t engine_font;

float last_stamina;
int32_t last_hp;

plugin_info_t info = {
    .pszName = "Rebuilt Simon",
    .pszDescription = "Rewrite of MadSimon by kohtep",
    .pszContact = "Don't",
    .nVerMajor = 1,
    .nVerMinor = 1,
    .nBuild = 1
};

bool initialize_common_globals(void) {
    g_hw_base = GetModuleHandleA("hw.dll");
    g_hl_base = GetModuleHandleA("hl.dll");
    if (!g_hl_base || !g_hw_base) return false;
    //g_CoF.jump_dodge_stamina_reduction_amount = (float*)OFFSET(g_hl_base, 0x001CEB90);
    //g_CoF.initial_sprint_stamina_reduction_amount = (float*)OFFSET(g_hl_base, 0x001A9724);
    //g_CoF.sprint_stamina_reduction_amount = (float*)OFFSET(g_hl_base, 0x001A1380);
    g_client_base = (HMODULE)OFFSET(g_hw_base, 0x1F02AC);
    g_CoF.pmove = *(playermove_t **)OFFSET(g_hl_base, 0x220C50);

    return true;
}

#ifndef COF_MANAGER
bool initialize_globals(void) {
    /* TODO: use variables/macros for offsets */
    /* Offsets can go to a separate header file */
    g_CoF.pEngine = (cl_enginefunc_t*)OFFSET(g_hw_base, 0x001A9A90);
    g_CoF.pServer = (enginefuncs_t*)OFFSET(g_hw_base, 0x001CED18);
    g_CoF.pClient = (cldll_func_t*)OFFSET(g_hw_base, 0x012B9BE0);
    g_CoF.pgEntityInterface = (DLL_FUNCTIONS*)OFFSET(g_hw_base, 0x0080D8C0);
    g_CoF.pClientState = (client_static_t*)OFFSET(g_hw_base, 0x010CF280);
    g_CoF.pServerState = (server_static_t*)OFFSET(g_hw_base, 0x0081F320);
    g_CoF.gclmove = (playermove_t*)OFFSET(g_hw_base, 0x0107F7A0);
    g_CoF.gsv_player = (edict_t**)OFFSET(g_hw_base, 0x0080E07C);

    return true;
}
#endif /* !COF_MANAGER */