#ifndef GLOBALS_H_
#define GLOBALS_H_

#include "pch.h"
#include "SDK/structures.h"
#include "SDK/Helpers/Parsers/res_parser.h"
#include "SDK/Logger/logger.h"

extern HMODULE g_hl_base;
extern HMODULE g_hw_base;
extern HMODULE g_client_base;
extern cof_data_t g_CoF;
extern plugin_info_t info;
extern SCREENINFO screen_info;
extern cl_entity_t* localplayer;

extern float last_stamina;
extern int32_t last_hp;

bool initialize_common_globals(void);
bool initialize_globals(void);

#endif /* GLOBALS_H_ */