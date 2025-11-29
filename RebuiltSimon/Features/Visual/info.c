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

uint8_t info_background_color[4];

/* Find a better way to set these values when they change rather than at each frame */
/* Still didn't found a way to do this better */
static void refresh_info_cvar_variables() {
    /* Set info color */
    parse_rgba_u8(CVAR_STRING_VALUE(info_background_color), info_background_color, CVAR_DEFAULT_STR_info_background_color);
    uint8_t color[3];
    parse_rgb_u8(CVAR_STRING_VALUE(info_color), color, CVAR_DEFAULT_STR_info_color);
    info_style.color[0] = (float)color[0]/255;
    info_style.color[1] = (float)color[1]/255;
    info_style.color[2] = (float)color[2]/255;

    /* Offset from the top row */
    info_style.index = (int)CVAR_FLOAT_VALUE(info_offset);
    /* Text time to live */
    info_style.time_to_live = CVAR_FLOAT_VALUE(info_ttl);
}

void rebuilt_simon_info_HUD_Redraw(double time) {
    if (CVAR_OFF(info)) return;
    if (!in_game()) return;
    if (CVAR_ON(info_background)) {
        int info_offset = (int)CVAR_FLOAT_VALUE(info_offset);
        int info_background_y = NPRINTF_Y_MARGIN + NPRINTF_FONT_HEIGHT * info_offset - INFO_BACKGROUND_Y_MARGIN;
        int info_background_height = last_frame_info_line_count * NPRINTF_FONT_HEIGHT + INFO_BACKGROUND_Y_MARGIN * 2;
        int info_background_x = screen_info.iWidth - INFO_BACKGROUND_WIDTH;
        g_CoF.pEngine->pfnFillRGBABlend(info_background_x, info_background_y, INFO_BACKGROUND_WIDTH, info_background_height, info_background_color[0], info_background_color[1], info_background_color[2], info_background_color[3]);
    }
}
void rebuilt_simon_info_HUD_Frame(double time) {
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
    DRAW_STATUS_TEXT("Version: %d.%d.%d", info.nBuild, info.nVerMajor, info.nVerMinor);
    ++info_style.index;


    if (CVAR_ON(info_stats)) {
        bool hp = CVAR_ON(info_health), stamina = CVAR_ON(info_stamina), speed = CVAR_ON(info_speed);
        bool stats_visible = (hp || stamina || speed);
        if (stats_visible) DRAW_STATUS_TEXT(".::  Simon Info  ::.");

        if (hp) {
            DRAW_STATUS_TEXT("Health:    %d", get_health());
        }
        if (stamina) {
            DRAW_STATUS_TEXT("Stamina:   %d", get_stamina());
        }
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
