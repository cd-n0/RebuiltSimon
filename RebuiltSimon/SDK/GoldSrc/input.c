#include "RebuiltSimon/pch.h"
#include "RebuiltSimon/globals.h"
#include <stdlib.h>

kbutton_t in_ducktap;

/* https://github.com/ValveSoftware/halflife/blob/b1b5cf5892918535619b2937bb927e46cb097ba1/cl_dll/input.cpp#L327 */
/*
============
KeyDown
============
*/
void KeyDown(kbutton_t* b)
{
	int k;
	const char* c;

	c = g_CoF.pEngine->Cmd_Argv(1);
	if (c[0])
		k = atoi(c);
	else
		k = -1;		// typed manually at the console for continuous down

	if (k == b->down[0] || k == b->down[1])
		return;		// repeating key

	if (!b->down[0])
		b->down[0] = k;
	else if (!b->down[1])
		b->down[1] = k;
	else
	{
		g_CoF.pEngine->Con_DPrintf("Three keys down for a button '%c' '%c' '%c'!\n", b->down[0], b->down[1], c);
		return;
	}

	if (b->state & 1)
		return;		// still down
	b->state |= 1 + 2;	// down + impulse down
}

/*
============
KeyUp
============
*/
void KeyUp(kbutton_t* b)
{
	int k;
	const char* c;

	c = g_CoF.pEngine->Cmd_Argv(1);
	if (c[0])
		k = atoi(c);
	else
	{ // typed manually at the console, assume for unsticking, so clear all
		b->down[0] = b->down[1] = 0;
		b->state = 4;	// impulse up
		return;
	}

	if (b->down[0] == k)
		b->down[0] = 0;
	else if (b->down[1] == k)
		b->down[1] = 0;
	else
		return;		// key up without coresponding down (menu pass through)
	if (b->down[0] || b->down[1])
	{
		//Con_Printf ("Keys down for button: '%c' '%c' '%c' (%d,%d,%d)!\n", b->down[0], b->down[1], c, b->down[0], b->down[1], c);
		return;		// some other key is still holding it down
	}

	if (!(b->state & 1))
		return;		// still up (this should not happen)

	b->state &= ~1;		// now up
	b->state |= 4; 		// impulse up
}


void IN_ducktapUp(void)   { KeyUp(&in_ducktap); }
void IN_ducktapDown(void) { KeyDown(&in_ducktap); }