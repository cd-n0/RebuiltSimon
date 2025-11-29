#include "RebuiltSimon/cvars.h"
#include "RebuiltSimon/globals.h"
#include "RebuiltSimon/SDK/Helpers/SpriteUtils/sprite_utils.h"
#include "RebuiltSimon/SDK/Helpers/Entities/player.h"
#include "RebuiltSimon/SDK/Helpers/Parsers/parsers.h"
#include <stddef.h>

typedef struct hud_style_s {
    uint8_t color[3];
    int     speedometer_position[2];
    int     jumpspeed_position[2];
    int     health_position[2];
    int     stamina_position[2];
} hud_style_t;

static hud_style_t hud_style;


/* Find a better way to set these values when they change rather than at each frame */
static void refresh_hud_cvar_variables() {
    parse_rgb_u8(CVAR_STRING_VALUE(hud_color), hud_style.color, CVAR_DEFAULT_STR_hud_color);
    parse_normalized_pos(CVAR_STRING_VALUE(hud_speedometer_position), hud_style.speedometer_position, CVAR_DEFAULT_STR_hud_speedometer_position);
    parse_normalized_pos(CVAR_STRING_VALUE(hud_jumpspeed_position), hud_style.jumpspeed_position, CVAR_DEFAULT_STR_hud_jumpspeed_position);
    parse_normalized_pos(CVAR_STRING_VALUE(hud_health_position), hud_style.health_position, CVAR_DEFAULT_STR_hud_health_position);
    parse_normalized_pos(CVAR_STRING_VALUE(hud_stamina_position), hud_style.stamina_position, CVAR_DEFAULT_STR_hud_stamina_position);
}


void rebuilt_simon_hud_HUD_Redraw(double time) {
    if (!in_game()) return;

    if (CVAR_OFF(hud)) return;
    refresh_hud_cvar_variables();

    if (CVAR_ON(hud_speedometer)) {
        vec3_t velocity = { 0 };
        memcpy(velocity, g_CoF.gclmove->velocity, (sizeof(float) * 2));
        draw_number((int)VectorLength(velocity), hud_style.speedometer_position[0], hud_style.speedometer_position[1],
            hud_style.color[0], hud_style.color[1], hud_style.color[2]);
    }
    if (CVAR_ON(hud_jumpspeed)) {
        static vec3_t previous_velocity = { 0 };
        static int previous_speed_2d;
        if ((g_CoF.gclmove->velocity[2] != 0.0f && previous_velocity[2] == 0.0f) || (g_CoF.gclmove->velocity[2] > 0.0f && previous_velocity[2] < 0.0f)) {
            previous_velocity[2] = 0;
            previous_speed_2d = (int)VectorLength(previous_velocity);
        }
        draw_number(previous_speed_2d, hud_style.jumpspeed_position[0], hud_style.jumpspeed_position[1],
            hud_style.color[0], hud_style.color[1], hud_style.color[2]);

        memcpy(previous_velocity, g_CoF.gclmove->velocity, sizeof(float) * 3);
    }

     /* last_hp and last_stamina set at info */
     if (CVAR_ON(hud_health)) {
         draw_number(get_health(), hud_style.health_position[0], hud_style.health_position[1],
             hud_style.color[0], hud_style.color[1], hud_style.color[2]);
     }

     if (CVAR_ON(hud_stamina)) {
         draw_number(get_stamina(), hud_style.stamina_position[0], hud_style.stamina_position[1],
             hud_style.color[0], hud_style.color[1], hud_style.color[2]);
     }
}
