#include "RebuiltSimon/globals.h"
#include "RebuiltSimon/Features/Movement/autojump.h"
#include "RebuiltSimon/Features/Movement/ducktap.h"
#include "RebuiltSimon/Features/Movement/yaw.h"

typedef void(__cdecl* CL_CreateMove_t)(float frametime, struct usercmd_s* cmd, int active);
static CL_CreateMove_t CL_CreateMove_o;

static void CL_CreateMove_h(float frametime, usercmd_t* cmd, int active) {
    CL_CreateMove_o(frametime, cmd, active);
    autojump(cmd);
    ducktap(cmd);
    yaw(cmd);
}

void hook_CL_CreateMove() {
    CreateHook(g_CoF.pClient->CL_CreateMove, CL_CreateMove_h, &CL_CreateMove_o);
}