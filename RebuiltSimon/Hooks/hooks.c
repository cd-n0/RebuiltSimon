#include "RebuiltSimon/globals.h"
#include "RebuiltSimon/Features/QOL/rawinput.h"
#include "hooks.h"

static WNDPROC wndproc_o;

static DWORD CALLBACK wndproc_h(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
    switch (msg) {
        case WM_INPUT:
            process_rawinput(msg, wParam, lParam);
    }

    return CallWindowProcA(wndproc_o, hwnd, msg, wParam, lParam);
}

bool initialize_hooks(void) {
    MH_Initialize();

    for (size_t i = 0; i < NUM_HOOKS; ++i) {
        hook_inits[i]();
    }

    if (MH_OK != MH_EnableHook(MH_ALL_HOOKS)) {
        g_CoF.pEngine->Con_Printf("Failed hooking functions\n");
        return false;
    }
    wndproc_o = SetWindowLongPtrA(g_CoF.hwnd, GWLP_WNDPROC, wndproc_h);

    return true;
}

void deinitialize_hooks(void) {
    SetWindowLongPtrA(g_CoF.hwnd, GWLP_WNDPROC, wndproc_o);
    MH_Uninitialize();
}