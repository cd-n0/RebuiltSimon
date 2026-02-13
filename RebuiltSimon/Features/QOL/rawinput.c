#include "RebuiltSimon/pch.h"
#include "RebuiltSimon/cvars.h"
#include "RebuiltSimon/globals.h"

bool initialize_rawinput() {
    RAWINPUTDEVICE device = {
        .usUsagePage = HID_USAGE_PAGE_GENERIC,
        .usUsage = HID_USAGE_GENERIC_MOUSE,
        .hwndTarget = g_CoF.hwnd
    };

    return RegisterRawInputDevices(&device, 1, sizeof(device));
}


void deinitialize_rawinput(void) {
    RAWINPUTDEVICE device = {
        .usUsagePage = HID_USAGE_PAGE_GENERIC,
        .usUsage = HID_USAGE_GENERIC_MOUSE,
        .dwFlags = RIDEV_REMOVE
    };
    
    RegisterRawInputDevices(&device, 1, sizeof(device));
}

void process_rawinput(UINT msg, WPARAM wParam, LPARAM lParam)
{
	if (CVAR_OFF(rawinput))
		return;

	CURSORINFO ci = { sizeof(ci) };
	if (!GetCursorInfo(&ci) || (ci.flags & CURSOR_SHOWING)) {
		return;
	}

	BYTE buffer[sizeof(RAWINPUT)];
	UINT size = sizeof(buffer);

	const UINT received = GetRawInputData(
		(HRAWINPUT)lParam,
		RID_INPUT,
		buffer,
		&size,
		sizeof(RAWINPUTHEADER));

	if (received == -1 || received > sizeof(buffer))
		return;

	const RAWINPUT* raw = (RAWINPUT*)buffer;

	if (raw->header.dwType != RIM_TYPEMOUSE)
		return;

	const LONG dx = raw->data.mouse.lLastX;
	const LONG dy = raw->data.mouse.lLastY;

	g_CoF.m_accum->x += dx;
	g_CoF.m_accum->y += dy;
}
