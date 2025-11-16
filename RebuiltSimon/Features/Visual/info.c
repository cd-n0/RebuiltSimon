#include "RebuiltSimon/cvars.h"
#include "RebuiltSimon/globals.h"
#include "RebuiltSimon/SDK/Helpers/Entities/player.h"
#include "RebuiltSimon/SDK/Helpers/Parsers/parsers.h"

#define DRAW_STATUS_TEXT(...) do { \
g_CoF.pEngine->Con_NXPrintf(&info_style, __VA_ARGS__); \
++info_style.index; \
} while (0)

int last_frame_info_line_count;
static con_nprint_t info_style = { 0 };

/* Find a better way to set these values when they change rather than at each frame */
/* Still didn't found a way to do this better */
static void refresh_info_cvar_variables() {
    /* Set info color */
    uint8_t color[3];
    parse_rgb_u8(CVAR_STRING_VALUE(info_color), color, CVAR_DEFAULT_STR_info_color);
    info_style.color[0] = (float)color[0]/255;
    info_style.color[1] = (float)color[1]/255;
    info_style.color[2] = (float)color[2]/255;

    /* Offset from the top row */
    info_style.index = (int)CVAR_FLOAT_VALUE(info_offset);
    /* Text time to live */
    info_style.time_to_live = CVAR_FLOAT_VALUE(info_ttl);

    /* Infinite stamina */
    if (g_CoF.gsv_player) {
        edict_t** dict = g_CoF.gsv_player;
        if (dict) {
            simon_t* simon = (simon_t*)(*dict)->pvPrivateData;
            if (simon) { simon->infinite_stamina = CVAR_ON(disable_stamina) ? 1 : 0; }
        }
    };
}

void rebuilt_simon_info_frame(double time) {
    /* Draw info */
    if (CVAR_OFF(info)) return;
    if (!in_game()) return;

    refresh_info_cvar_variables();

    /* Format CPU time */
    ULONGLONG msTotal = GetTickCount64();
    DWORD hours = msTotal / (1000 * 60 * 60);
    DWORD minutes = (msTotal / (1000 * 60)) % 60;
    DWORD seconds = (msTotal / 1000) % 60;
    DWORD milliseconds = msTotal % 1000;

    DRAW_STATUS_TEXT("RebuiltSimon");
    DRAW_STATUS_TEXT("Build: %d%d%d", info.nBuild, info.nVerMajor, info.nVerMinor);
    ++info_style.index;


    if (CVAR_ON(info_stats)) {
        bool hp = CVAR_ON(info_health), stamina = CVAR_ON(info_stamina), speed = CVAR_ON(info_speed);
        bool stats_visible = (hp || stamina || speed);
        if (stats_visible) DRAW_STATUS_TEXT(".::  Simon Info  ::.");

#if 0
        //g_CoF.pEngine->Con_Printf("%p\n", g_CoF.pEngine->GetEntityByIndex(1));
        //g_CoF.pEngine->Con_Printf("1:%p 2:%p 3:%p 4:%p\n", get_player(1), get_player(2), get_player(3), get_player(4));
        /* ts so ass brah */
        /* This will only work for the host */
        simon_t* simon = get_player();
        if (simon) {
            if (simon->player_index == 1) {
                if (hp) {
                    DRAW_STATUS_TEXT("Health:    %3d", simon->hp_view);
                    last_hp = simon->hp_view;
                }
                if (stamina) {
                    DRAW_STATUS_TEXT("Stamina:   %3.f", simon->stamina);
                    last_stamina = simon->stamina;
                }
            }
            else {
                if (hp) {
                    DRAW_STATUS_TEXT("Health:    %3d", last_hp);
                }
                if (stamina) {
                    DRAW_STATUS_TEXT("Stamina:   %3.f", last_stamina);
                }
            }
        }
#else
        if (hp) {
            DRAW_STATUS_TEXT("Health:    %d", get_health());
        }
        if (stamina) {
            DRAW_STATUS_TEXT("Stamina:   %d", get_stamina());
        }
#endif
        if (speed) {
            vec3_t velocity = { 0 };
            memcpy(velocity, g_CoF.gclmove->velocity, (sizeof(float) * 2));
            DRAW_STATUS_TEXT("Speed:     %3.f", VectorLength(velocity));
        }

        if (stats_visible) ++info_style.index;
    }

    DRAW_STATUS_TEXT("%02lu:%02lu:%02lu.%03lu", hours, minutes, seconds, milliseconds);
    last_frame_info_line_count = info_style.index - (int)CVAR_FLOAT_VALUE(info_offset);
}
