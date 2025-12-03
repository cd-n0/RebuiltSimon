#include "pch.h"
#include "globals.h"
#include "rebuilt_simon.h"
#include "SDK/structures.h"

BOOL APIENTRY DllMain(HMODULE hModule, DWORD  ul_reason_for_call, LPVOID lpReserved)
{
    switch (ul_reason_for_call) {

#ifndef COF_MANAGER /* Attach behaviour is only needed on the injected version */
        case DLL_PROCESS_ATTACH: {
            DisableThreadLibraryCalls(hModule);

            if (!initialize_rebuilt_simon()) {
                MessageBoxA(0, "Failed at initialization", "Error", 0);
                return false;
            }

            fprintf(stdout, "RebuiltSimon injected\n");
        } break;
#endif /* !COF_MANAGER */
        case DLL_PROCESS_DETACH: {
            deinitialize_rebuilt_simon();
        } break;
    }
    return TRUE;
}

#ifdef COF_MANAGER

/* #define EXPORT __declspec(dllexport) */
typedef struct cof_manager_data_s
{
    int version;

    cl_enginefunc_t* pcl_enginefuncs;
    cldll_func_t* pcl_funcs;
    enginefuncs_t* psv_enginefuncs;

    DLL_FUNCTIONS* p_entityInterface;

    playermove_t* pcl_move;
    edict_t** psv_player;

    globalvars_t** gGlobals;

    client_static_t* pcls;
    server_static_t* psvc;
} cof_manager_data_t;

EXPORT void GetPluginInfo(plugin_info_t** plugin_info) {
    *plugin_info = &info;
}

EXPORT void GetGameVars(cof_manager_data_t* vars) {
    g_CoF.version = vars->version;
    g_CoF.pEngine = vars->pcl_enginefuncs;
    g_CoF.pClient = vars->pcl_funcs;
    g_CoF.pServer = vars->psv_enginefuncs;
    g_CoF.pgEntityInterface = vars->p_entityInterface;
    g_CoF.gclmove = vars->pcl_move;
    g_CoF.gsv_player = vars->psv_player;
    g_CoF.gGlobals = vars->gGlobals;

    g_CoF.pClientState = vars->pcls;
    g_CoF.pServerState = vars->psvc;
}

EXPORT bool Init() {
    if (g_CoF.version < 2) {
        g_CoF.pServerState = (server_static_t*)OFFSET(g_hw_base, 0x0081F320);
    }
    return initialize_rebuilt_simon();
}

#endif /* COF_MANAGER */