#include "pch.h"
#include "globals.h"
#include "cvars.h"
#include "Hooks/hooks.h"
#include "Features/Visual/hud.h"
#include "RebuiltSimon/SDK/Helpers/SpriteUtils/sprite_utils.h"
#include "RebuiltSimon/Features/QOL/rawinput.h"

bool initialize_rebuilt_simon(void) {
    if (!initialize_common_globals()) {
        MessageBoxA(0, "Failed initializing globals", "Error", 0);
        return false;
    };
#ifndef COF_MANAGER
    /* CoFManager version gets these from GetGameVars */
    if (!initialize_globals()) {
        MessageBoxA(0, "Failed initializing globals", "Error", 0);
        return false;
    };
#endif
    cvars_initialize();
    if (!initialize_hooks()) {
        MessageBoxA(0, "Failed initializing hooks", "Error", 0);
        return false;
    }

    /* Initialize screen info and sprites */
    screen_info.iSize = sizeof(screen_info);
    g_CoF.pEngine->pfnGetScreenInfo(&screen_info);
    if (!initialize_sprites()) {
        MessageBoxA(0, "Failed initializing sprites", "Error", 0);
        return false;
    }

    engine_font = get_engine_font(screen_info.iHeight);
    if (0 == engine_font.tall) {
        MessageBoxA(0, "Failed getting the engine font height", "Error", 0);
        return false;
    }

    if (!initialize_rawinput()) {
        MessageBoxA(0, "Failed initializing rawinput", "Error", 0);
        return false;
    }

    /* Reload archived values */
    g_CoF.pEngine->pfnClientCmd("exec config.cfg");

    g_CoF.pEngine->Con_Printf("RebuiltSimon initialized successfully\n");

    return true;
}


void deinitialize_rebuilt_simon(void) {
    deinitialize_hooks();
    cvars_deinitialize();
    deinitialize_rawinput();
}