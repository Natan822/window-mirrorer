#include <iostream>
#include <assert.h>
#include "CreateWindow.h"
#include "global.h"
#include "Renderer.h"
#include "WindowsManager.h"

HWND mainWindowHandle;
HWND targetHandle;
RECT targetRect;

int windowX = 0;
int windowY = 0;

const bool CONSOLE_ENABLED = false;

int main() {

	if (CONSOLE_ENABLED)
	{
		::AllocConsole();
		FILE* pCout;
		freopen_s(&pCout, "CONOUT$", "w", stdout);
	}
	
	::EnumWindows(enumWindowsProc, NULL);

	listWindows();

	std::cout << "Type in the window to be mirrored: ";
	int input;
	std::cin >> input;
	std::cin.get();

	//mainWindowHandle = CreateMainWindow(hInstance);
	mainWindowHandle = ::CreateMainWindow(NULL);

	if (mainWindowHandle == NULL) 
	{
		return 0;
	}

	//ShowWindow(mainWindowHandle, nCmdShow);
	::ShowWindow(mainWindowHandle, SW_SHOW);

	targetHandle = windowsHandles.at(input - 1);
	HDC targetDC = ::GetDC(targetHandle);
	HDC mainDC = ::GetDC(mainWindowHandle);

	::GetWindowRect(targetHandle, &targetRect);

	INT_PTR target = (input - 1);
	::CreateThread(NULL, 0, mirrorWindow, reinterpret_cast<LPVOID>(target), 0, NULL);

	MSG msg;
	while (GetMessage(&msg, mainWindowHandle, 0, 0))
	{
		::TranslateMessage(&msg);
		::DispatchMessage(&msg);
	}

	return 1;
}

