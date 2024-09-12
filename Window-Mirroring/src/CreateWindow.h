#pragma once
#include <Windows.h>

extern LRESULT CALLBACK WindowProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam);

extern HWND CreateMainWindow(HINSTANCE hInstance);