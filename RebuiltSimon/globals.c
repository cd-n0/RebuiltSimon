#include "pch.h"
#include "SDK/structures.h"
#include "globals.h"
#include "SDK/GoldSrc/Wrappers/IGame.h"

HMODULE g_hl_base;
HMODULE g_hw_base;
HMODULE g_client_base;
cof_data_t g_CoF;
SCREENINFO screen_info;
cl_entity_t* localplayer;

float last_stamina;
int32_t last_hp;

plugin_info_t info = {
    .pszName = "Rebuilt Simon",
    .pszDescription = "Rewrite of MadSimon by kohtep",
    .pszContact = "Don't",
    .nVerMajor = 2,
    .nVerMinor = 3,
    .nBuild = 1
};

bool initialize_common_globals(void) {
    if (!(g_hw_base = GetModuleHandleA("hw.dll"))) { g_hw_base = LoadLibraryA("hw.dll"); }
    if (!(g_hl_base = GetModuleHandleA("hl.dll"))) { g_hl_base = LoadLibraryA("hl.dll"); }
    if (!g_hl_base || !g_hw_base) return false;
    g_client_base = *(void**)(HMODULE)OFFSET(g_hw_base, 0x1F02AC);
    g_CoF.pmove = *(playermove_t **)OFFSET(g_hl_base, 0x220C50);
    void* game = *(void**)OFFSET(g_hw_base, 0x1D08CC);
    g_CoF.hwnd = GetMainWindow(game);
    g_CoF.m_accum = OFFSET(g_client_base, 0x543580);

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