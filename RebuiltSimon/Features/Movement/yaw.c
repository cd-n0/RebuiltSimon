#include "RebuiltSimon/globals.h"
#include "RebuiltSimon/cvars.h"
#include "RebuiltSimon/SDK/GoldSrc/input.h"

void yaw(usercmd_t *cmd) {
	if (in_left.state & 1) {
		cmd->viewangles[1] += CVAR_FLOAT_VALUE(yawspeed) / 100;
		g_CoF.pEngine->SetViewAngles(cmd->viewangles);
	}
	if (in_right.state & 1) {
		cmd->viewangles[1] -= CVAR_FLOAT_VALUE(yawspeed) / 100;
		g_CoF.pEngine->SetViewAngles(cmd->viewangles);
	}
}