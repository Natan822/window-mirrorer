#include <iostream>

#include "CreateWindow.h"
#include "WindowsManager.h"
#include "global.h"
#include "vector"

std::vector <HWND> windowsHandles = {};

// Inserts all currently open windows into windowsHandles vector
BOOL CALLBACK enumWindowsProc(HWND hwnd, LPARAM lParam) {
	int length = ::GetWindowTextLengthA(hwnd) + 1;

	if (length > 1 && IsWindowVisible(hwnd))
	{
		char* buffer = new char[length];
		::GetWindowTextA(hwnd, buffer, length);

		//std::cout << buffer << std::endl;

		windowsHandles.push_back(hwnd);

		delete[] buffer;
	}

	return TRUE;
}

void listWindows() {
	int length = 0;
	for (size_t i = 0; i < windowsHandles.size(); i++)
	{
		HWND windowHandle = windowsHandles.at(i);
		length = ::GetWindowTextLengthA(windowHandle) + 1;

		char* buffer = new char[length];
		::GetWindowTextA(windowHandle, buffer, length);

		std::cout << (i + 1) << ". " << buffer << std::endl;

		delete[] buffer;
	}
	std::cout << std::endl;
}