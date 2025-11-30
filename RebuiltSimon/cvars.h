#ifndef CVARS_H_
#define CVARS_H_

#include "pch.h"

typedef void (*xcommand_t)(void);
typedef struct cmd_function_s {
    struct cmd_function_s* next;
    char* name;
    xcommand_t function;
    int flags;
} cmd_function_t;

#define CVAR_PREFIX  "rs_"

#define UNUSED_MACRO_PARAMETER(var)

#define DEFINE_CVAR_DEFAULT(name, value) \
const char *CVAR_DEFAULT_STR_##name = #value
#define DECLARE_CVAR(name, value) cvar_t* cvar_rs_##name = NULL; UNUSED_MACRO_PARAMETER(value); DEFINE_CVAR_DEFAULT(name,value)

#define DEFINE_CVAR_DEFAULT_EXTERN(name) \
extern const char *CVAR_DEFAULT_STR_##name
#define DECL_CVAR_EXTERN(name, value) extern cvar_t* cvar_rs_##name;  UNUSED_MACRO_PARAMETER(value); DEFINE_CVAR_DEFAULT_EXTERN(name)

enum { FCVAR_RS = 1 << 29 /* Unused by GoldSrc/CoF */ };

#define REGISTER_CVAR(name, value) \
    cvar_rs_##name = g_CoF.pEngine->pfnRegisterVariable(CVAR_PREFIX #name, #value, FCVAR_RS | FCVAR_ARCHIVE);

#define CVAR_ON(name) (cvar_rs_##name->value != 0.0f)
#define CVAR_OFF(name) (cvar_rs_##name->value == 0.0f)
#define CVAR_FLOAT_VALUE(name) cvar_rs_##name->value
#define CVAR_STRING_VALUE(name) cvar_rs_##name->string


/* Declaring extern, declaring and registering on all cvars was exhaustive so I added a value paramater to the other 2 macros and did all 3 operations with this after 17 cvars
 * I don't like that 2 of the macros has an unused paramater but remembering and writing everything in 3 different places each time I add a cvar was really exhaustive */
//OPERATION(eastereggs, 0);                         /* Secret IycGjfwHXac */
#define CVAR_OPERATION(OPERATION)                                                                                           \
/* Info Cvars */                                                                                                            \
OPERATION(info, 1);                               /* Enable the info */                                                     \
OPERATION(info_offset, 4);                        /* Info offset from the 0th row */                                        \
OPERATION(info_color, 255 255 255);               /* Info color as "r g b" values ranging from 0 to 255 */                  \
OPERATION(info_background, 1);                    /* Enable info background */                                              \
OPERATION(info_background_color, 20 20 20 160);   /* Info background color as "r g b a" values ranging from 0 to 255 */     \
OPERATION(info_stats, 1);                         /* Enable simons stats at the info panel */                               \
OPERATION(info_ttl, 0.1);                         /* Time to live variable for the info text */                             \
OPERATION(info_health, 1);                        /* Enable health on the info panel */                                     \
OPERATION(info_stamina, 1);                       /* Enable stamina on the info panel */                                    \
OPERATION(info_speed, 1);                         /* Enable speed on the info panel */                                      \
/* HUD Cvars */                                                                                                             \
OPERATION(hud, 1);                                /* Enable the hud */                                                      \
OPERATION(hud_color, 255 255 255);                /* HUD color as "r g b" values ranging from 0 to 255 */                   \
OPERATION(hud_speedometer, 1);                    /* Enable the speedometer */                                              \
OPERATION(hud_speedometer_position, .5 .9);       /* Normalized position of the speedometer */                              \
OPERATION(hud_jumpspeed, 1);                      /* Enable the jumpspeed */                                                \
OPERATION(hud_jumpspeed_position, .5 .95);        /* Normalized position of the jumpspeed */                                \
OPERATION(hud_stamina, 0);                        /* Enable the stamina */                                                  \
OPERATION(hud_stamina_position, .07 .73);         /* Normalized position of the stamina */                                  \
OPERATION(hud_health, 0);                         /* Enable the speedometer */                                              \
OPERATION(hud_health_position, .02 .73);          /* Normalized position of */                                              \
OPERATION(hud_draw_centered, 1);                  /* Draw the numbers centered */                                           \
                                                                                                                            \
OPERATION(bhop_cap, 1);                           /* Enable bhop cap */                                                     \
OPERATION(cof_save_prevention, 1);                /* Enable save prevention */                                              \
OPERATION(disable_stamina, 0);                    /* Disable stamina */                                                     \
OPERATION(disable_duck_time, 0);                  /* Disable duck time (enable ducktap) */                                  \
OPERATION(autojump, 0);                           /* Enable autojump */                                                     \
OPERATION(ducktap_priority, 1);                   /* Prioritize ducktap over autojump */                                    \
                                                                                                                            \
OPERATION(yawspeed, 210);                           /* +rs_left and +rs_right speed */                                      \
OPERATION(rawinput, 1);                             /* rawinput */                                                          \
OPERATION(absolute_speed, 0);                       /* 0 for horizontal speed */                                            \
//OPERATION(crosshair, 0);                          /* Enable crosshair */                                                    \
//OPERATION(crosshair_dynamic, 0);                  /* Change crosshair gap based on velocity */                              \
//OPERATION(crosshair_dot, 0);                      /* Enable crosshair dot */                                                \
//OPERATION(crosshair_gap, 5);                      /* Crosshair gap */                                                       \
//OPERATION(crosshair_t, 0);                        /* Don't draw the top line */                                             \
//OPERATION(crosshair_length, 8);                   /* Crosshair length */                                                    \
//OPERATION(crosshair_thickness, 1);                /* Crosshair thickness */                                                 \
//OPERATION(crosshair_color, 0 255 0 255);          /* Crosshair color as "r g b a" values ranging from 0 to 255  */          \



CVAR_OPERATION(DECL_CVAR_EXTERN);

extern cmd_function_t** cmd_functions;
extern cvar_t** cvar_vars;

void cvars_initialize(void);
void cvars_deinitialize(void);

#endif /* CVARS_H_ */