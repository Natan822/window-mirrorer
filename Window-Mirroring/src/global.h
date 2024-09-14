#pragma once
#include <vector>
#include "Renderer.h"

extern std::vector <HWND> windowsHandles;

extern const bool CONSOLE_ENABLED;

extern int windowX;
extern int windowY;

extern HWND mainWindowHandle;
extern RECT mainRect;

extern HWND targetHandle;
extern RECT targetRect;

extern MirrorType mirrorType;