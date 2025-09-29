#ifndef HOOKS_H_
#define HOOKS_H_

#include "RebuiltSimon/pch.h"

/* TODO: Too much repetition/pattern in hook files, do something about it */
/* TODO: Too much macro magic */

// Declare all hook init functions as extern
#define X(name) void name(void);
#include "Server/hook_list.h"
#include "Client/hook_list.h"
#undef X

// Array of pointers to all hook init functions
static void (* const hook_inits[])(void) = {
#define X(name) name,
#include "Server/hook_list.h"
#include "Client/hook_list.h"
#undef X
};

#define NUM_HOOKS (sizeof(hook_inits)/sizeof(hook_inits[0]))

bool initialize_hooks(void);
void deinitialize_hooks(void);


#endif /* HOOKS_H_ */