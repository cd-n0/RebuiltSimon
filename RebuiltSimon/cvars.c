#include "pch.h"
#include "globals.h"
#include "cvars.h"
#include "SDK/GoldSrc/input.h"
#include "SDK/GoldSrc/convars.h"

CVAR_OPERATION(DECLARE_CVAR);

cmd_function_t** cmd_functions;
cvar_t** cvar_vars;

#ifndef COF_MANAGER
static void unload(void) {
	CreateThread(0, 0, FreeLibrary, GetModuleHandleA("RebuiltSimon.dll"), 0, 0);
}
#endif

void cvars_initialize(void) {
	cmd_functions = (cmd_function_t **)OFFSET(g_hw_base, 0x0036B290);
	cvar_vars = (cvar_t**)OFFSET(g_hw_base, 0x0037B034);
	CVAR_OPERATION(REGISTER_CVAR);
	Cmd_AddCommandWithFlags("+"CVAR_PREFIX"ducktap", IN_ducktapDown, FCVAR_RS);
	Cmd_AddCommandWithFlags("-"CVAR_PREFIX"ducktap", IN_ducktapUp,	 FCVAR_RS);

#ifndef COF_MANAGER
	Cmd_AddCommandWithFlags(CVAR_PREFIX"unload", unload, FCVAR_RS);
#endif
}

void cvars_deinitialize(void) {
	g_CoF.pEngine->Con_Printf("Unregistering functions\n");
	/* Advance head until we find a command without the flag */
	for (;*cmd_functions && (((*cmd_functions)->flags & FCVAR_RS) != 0); *cmd_functions = (*cmd_functions)->next);

	for (cmd_function_t* current_cmd = *cmd_functions, *last_kept_cmd = *cmd_functions; current_cmd != NULL;) {
		cmd_function_t* next = current_cmd->next;  /* Current one will get freed or changed so we store the next at the start */
		if ((current_cmd->flags & FCVAR_RS) != 0) {
			g_CoF.pEngine->Con_Printf("Command %s removed\n", current_cmd->name);
			last_kept_cmd->next = current_cmd->next;
			free(current_cmd);
		}
		else {
			last_kept_cmd = current_cmd;
		}
		current_cmd = next;
	}

	const char* Z_Free_PATTERN = "55 8B EC 83 EC ? 83 7D ? ? 75 ? 68 ? ? ? ? E8 ? ? ? ? 83 C4 ? 8B 45 ? 83 E8";
	void (*Z_Free)(void*) = (void (*)(void*))cdmem_find_signature("hw.dll", Z_Free_PATTERN);

	/* It crashes if we can't free anyway so we don't need to check if we got the pattern or not */
	/*
	if (!Z_Free) {
		g_CoF.pEngine->Con_Printf("Can't find Z_Free\n");
		return;
	}
	*/

	g_CoF.pEngine->Con_Printf("Unregistering cvars\n", cvar_vars);
	/* Advance head until we find a variable without the flag */
	for (;*cvar_vars && (((*cvar_vars)->flags & FCVAR_RS) != 0); *cvar_vars = (*cvar_vars)->next);

	for (cvar_t* current_cvar = *cvar_vars, *last_kept_cvar = *cvar_vars; current_cvar != NULL;) {
		cvar_t* next = current_cvar->next;  /* Current one will get freed or changed so we store the next at the start */
		if ((current_cvar->flags & FCVAR_RS) != 0) {
			g_CoF.pEngine->Con_Printf("Cvar %s removed\n", current_cvar->name);
			last_kept_cvar->next = current_cvar->next;
			Z_Free(current_cvar->string);
			Z_Free(current_cvar);
		}
		else {
			last_kept_cvar = current_cvar;
		}
		current_cvar = next;
	}
}