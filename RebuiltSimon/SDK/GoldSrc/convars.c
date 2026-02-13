#include "RebuiltSimon/pch.h"
#include "RebuiltSimon/globals.h"
#include "RebuiltSimon/cvars.h"

/* Stub function if we don't specify a function while adding a command */
static void cmd_function_stub(void) {
    OutputDebugStringA("Cmd function stub");
}

void Cmd_AddCommandWithFlags(char* cmd_name, xcommand_t function, int flags) {
    for (cvar_t* current = *cvar_vars; current != NULL; current = current->next) {
        if (strcmp(cmd_name, current->name) == 0) {
            LOG_WARN("Cmd_AddCommand: %s already defined as a cvar not registering", cmd_name);
            return;
        }
    }
    for (cmd_function_t* current = *cmd_functions; current != NULL; current = current->next) {
        if (strcmp(cmd_name, current->name) == 0) {
            LOG_WARN("Cmd_AddCommand: %s already defined as a command not registering", cmd_name);
            return;
        }
    }

    cmd_function_t* newcmd = malloc(sizeof(*newcmd));
    if (!newcmd) {
        LOG_ERROR("Cmd_AddCommand: failed to allocate memory for %s", cmd_name);
        return;
    }

    newcmd->name = cmd_name;
    newcmd->function = function ? function : cmd_function_stub;
    newcmd->flags = flags;
    newcmd->next = *cmd_functions;

    *cmd_functions = newcmd;
}