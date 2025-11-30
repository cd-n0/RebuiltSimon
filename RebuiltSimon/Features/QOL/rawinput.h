#ifndef RAWINPUT_H_
#define RAWINPUT_H_

#include "RebuiltSimon/pch.h"

bool initialize_rawinput();
void deinitialize_rawinput(void);
void process_rawinput(UINT msg, WPARAM wParam, LPARAM lParam);

#endif /* RAWINPUT_H_ */