#include "pch.h"
#include "globals.h"
#include "cvars.h"
#include "Hooks/hooks.h"
#include "Features/Visual/hud.h"
#include "RebuiltSimon/SDK/Helpers/SpriteUtils/sprite_utils.h"
#include "RebuiltSimon/Features/QOL/rawinput.h"
#include "RebuiltSimon/SDK/Clock/clock.h"
#include "SDK/Helpers/Parsers/engine_font.h"

#ifdef CONSOLE
FILE* conout_err, * conout, * conin;
#endif

bool initialize_rebuilt_simon(void) {
    initialize_logger();
    rsclock_t initialization_clock;
    clock_start(&initialization_clock);
#ifdef CONSOLE
    /* Maybe put opening and closing of the console as a COM as well */
    if (AllocConsole()) {
        conout = freopen("CONOUT$", "w", stdout);
        conout_err = freopen("CONOUT$", "w", stderr);
        conin = freopen("CONIN$", "r", stdin);
    }
#endif
    if (!initialize_common_globals()) {
        LOG_ERROR("Failed initializing common globals");
        return false;
    };
#ifndef COF_MANAGER
    /* CoFManager version gets these from GetGameVars */
    if (!initialize_globals()) {
        LOG_ERROR("Failed initializing globals");
        return false;
    };
#endif
    
    cvars_initialize();
    if (!initialize_hooks()) {
        LOG_ERROR("Failed initializing hooks");
        return false;
    }

    /* Initialize screen info and sprites */
    screen_info.iSize = sizeof(screen_info);
    g_CoF.pEngine->pfnGetScreenInfo(&screen_info);
    if (!initialize_sprites()) {
        LOG_ERROR("Failed initializing sprites");
        return false;
    }

    int height = get_engine_font_height(screen_info.iHeight);
    if (height == 0) {
        LOG_WARN("Failed getting engine font height, info background will not work correctly");
    }

    if (!initialize_rawinput()) {
        LOG_WARN("Failed initializing rawinput, rawinput will not work");
    }
    clock_update(&initialization_clock);

    /* Reload archived values */
    g_CoF.pEngine->pfnClientCmd("exec config.cfg");

    LOG_INFO("RebuiltSimon initialized successfully in %lf seconds", initialization_clock.elapsed);

    return true;
}


void deinitialize_rebuilt_simon(void) {
    deinitialize_hooks();
    cvars_deinitialize();
    deinitialize_rawinput();
    deinitialize_logger();

#ifdef CONSOLE
    if (FreeConsole()) {
        fclose(conout);
        fclose(conout_err);
        fclose(conin);
    }
#endif
}