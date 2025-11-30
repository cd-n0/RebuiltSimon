#ifndef PCH_H_
#define PCH_H_

#define WIN32_LEAN_AND_MEAN
#define HSPRITE WINDOWS_HSPRITE
#include <windows.h>
#include <hidusage.h>
/* Windows HSPRITE clash with the Sprite handle typedef HSPRITE (int) in engine/cdll_int.h */
#undef HSPRITE

#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>
#include <MinHook.h>
#define CreateHook(pTarget, pDetour, ppOriginal) MH_CreateHook(pTarget, pDetour, (void**)ppOriginal)
#include "memory/memory.h"

#define STDINT_H <stdint.h>

#include "halflife_sdk/common/const.h"
#include "halflife_sdk/common/cvardef.h"
#include "halflife_sdk/common/kbutton.h"
//#include "halflife_sdk/public/xash3d_mathlib.h"
#include "halflife_sdk/common/render_api.h"
#include "halflife_sdk/common/netadr.h"
#include "halflife_sdk/common/ref_params.h"
#include "halflife_sdk/common/cl_entity.h"

#include "halflife_sdk/engine/eiface.h"
#include "halflife_sdk/engine/edict.h"

#include "halflife_sdk/pm_shared/pm_defs.h"
#include "halflife_sdk/common/con_nprint.h"

#include "halflife_sdk/engine/progdefs.h"
#include "halflife_sdk/engine/cdll_int.h"
#include "halflife_sdk/engine/cdll_exp.h"

#endif /* PCH_H_ */
