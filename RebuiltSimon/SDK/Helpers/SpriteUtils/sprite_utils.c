#include "RebuiltSimon/pch.h"
#include "RebuiltSimon/globals.h"
#include "RebuiltSimon/cvars.h"

#define BASE_TEN_DIGIT_COUNT 10

static HSPRITE digit_sprites[BASE_TEN_DIGIT_COUNT];
static wrect_t digit_sprite_rects[BASE_TEN_DIGIT_COUNT];
static vec2_t digit_sprite_lengths[BASE_TEN_DIGIT_COUNT];
static client_sprite_t* digit_sprite_pointers[BASE_TEN_DIGIT_COUNT];

static const char* SPRITE_FILE_EXTENSION = ".spr";

bool initialize_sprites(void) {
    int sprite_count;
    int sprite_resolution = (screen_info.iWidth < 640) ? 320 : 640;
    client_sprite_t* sprite_list = g_CoF.pEngine->pfnSPR_GetList("sprites/hud.txt", &sprite_count);

    if (!sprite_list) return false;

    for (client_sprite_t* sprite = sprite_list; sprite < sprite_list + sprite_count; ++sprite) {
        if (sprite->iRes == sprite_resolution
            && strncmp(sprite->szName, "number_", 7) == 0
            && sprite->szName[8] == '\0'
            && isdigit(sprite->szName[7]))
        {
            int digit = sprite->szName[7] - '0'; /* ASCII to integer for the 8th character gives us the digit (number_<digit>) look at hud.txt to understand it better */
            digit_sprite_pointers[digit] = sprite;
            digit_sprite_rects[digit] = sprite->rc;
            /* Storing these just to not do the subtractions each time we want to find the length of it */
            digit_sprite_lengths[digit][0] = sprite->rc.right - sprite->rc.left;
            digit_sprite_lengths[digit][1] = sprite->rc.bottom - sprite->rc.top;

            char sprite_path[MAX_PATH] = "sprites/";
            size_t path_length = strlen(sprite_path);
            strncat(sprite_path + path_length, sprite->szSprite, MAX_PATH - path_length);
            path_length = strlen(sprite_path);
            strncat(sprite_path + path_length, SPRITE_FILE_EXTENSION, MAX_PATH - path_length);
            digit_sprites[digit] = g_CoF.pEngine->pfnSPR_Load(sprite_path);
        }
    }

    return true;
}

static void draw_digit(int digit, int x, int y, int r, int g, int b)
{
    /* WHY TF IS THIS NOT DRAWING ANYTHING */
    g_CoF.pEngine->pfnSPR_Set(digit_sprites[digit], r, g, b);
    g_CoF.pEngine->pfnSPR_DrawAdditive(0, x, y, &digit_sprite_rects[digit]); /* Assuming digits aren't animated sprites */
}

void draw_number(int number, int x, int y, int r, int g, int b)
{
    if (number <= 0) {
        draw_digit(0, x, y, r, g, b);
        return;
    }

    /* collect digits in a small buffer(most - significant first) */
    int digits[0x20]; /* more than enough for storing the digits of an integer */
    int n = 0;
    int tmp = number;
    while (tmp > 0) {
        digits[n++] = tmp % 10;
        tmp /= 10;
    }

    if (CVAR_ON(hud_draw_centered)) {
        int number_length = 0;
        for (int i = n - 1; i >= 0; --i) {
            int digit = digits[i];
            number_length += digit_sprite_lengths[digit][0];
        }
        x -= number_length / 2;
    }

    /* digits are LSB..MSB in buffer; draw MSB..LSB */
    for (int i = n - 1; i >= 0; --i) {
        int digit = digits[i];
        draw_digit(digit, x, y, r, g, b);
        x += digit_sprite_lengths[digit][0];
    }
}