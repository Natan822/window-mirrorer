#include "CreateWindow.h"
#include "global.h"
#include "Renderer.h"
#include "WindowsManager.h"

#include <dwmapi.h>
#include <iostream>

RECT mainRect;
HTHUMBNAIL thumbnail;
DWM_THUMBNAIL_PROPERTIES dskThumbProps;

const int SCROLLBAR_SIZE = 30;

//MirrorType mirrorType = BITBLT;
//MirrorType mirrorType = PRINT_WINDOW;
MirrorType mirrorType = DWM_THUMBNAIL;

DWORD WINAPI bitBltMirror(int target) {
	HDC targetDC = ::GetDC(targetHandle);
	HDC mainDC = ::GetDC(mainWindowHandle);

	while (true) {
		::GetWindowRect(targetHandle, &targetRect);
		::BitBlt(
			mainDC,
			0,
			0,
			targetRect.right,
			targetRect.bottom,
			targetDC,
			windowX,
			windowY,
			SRCCOPY
		);

		::Sleep(16);

	}
	return 1;
}

DWORD WINAPI printMirror(int target) {
	HDC targetDC = ::GetDC(targetHandle);
	HDC mainDC = ::GetDC(mainWindowHandle);

	while (true) {
		::PrintWindow(targetHandle, targetDC, PW_RENDERFULLCONTENT);
		::InvalidateRect(targetHandle, &targetRect, TRUE);

		::BitBlt(
			mainDC,
			0,
			0,
			targetRect.right,
			targetRect.bottom,
			targetDC,
			windowX,
			windowY,
			SRCCOPY
		);

		::Sleep(16);

	}
	return 1;
}

DWORD WINAPI dwmThumbnailMirror(int target) {
	
	::GetWindowRect(mainWindowHandle, &mainRect);

	// Adjust cropping out
	RECT rcSource;
	rcSource.left = 0;
	rcSource.top = 0;
	rcSource.right = mainRect.right - mainRect.left - SCROLLBAR_SIZE;
	rcSource.bottom = mainRect.bottom - mainRect.top - (SCROLLBAR_SIZE * 1.8);

	// Define the destination rectangle where the thumbnail will be displayed
	RECT rcDestination;
	rcDestination.left = 0;
	rcDestination.top = 0;
	rcDestination.right = rcSource.right;
	rcDestination.bottom = rcSource.bottom;

	dskThumbProps.dwFlags = DWM_TNP_RECTSOURCE | DWM_TNP_RECTDESTINATION | DWM_TNP_VISIBLE | DWM_TNP_SOURCECLIENTAREAONLY;

	dskThumbProps.fSourceClientAreaOnly = TRUE;
	dskThumbProps.fVisible = TRUE;
	dskThumbProps.rcDestination = rcDestination;
	dskThumbProps.rcSource = rcSource;

	thumbnail = NULL;
	::DwmRegisterThumbnail(mainWindowHandle, targetHandle, &thumbnail);
	while (true) {
		// Display the thumbnail
		::DwmUpdateThumbnailProperties(thumbnail, &dskThumbProps);
		::Sleep(16);
	}
	return 1;
}

DWORD WINAPI mirrorWindow(LPVOID lParam) {
	int target = reinterpret_cast<int>(lParam);

	switch (mirrorType)
	{
	case BITBLT:
		return bitBltMirror(target);

	case PRINT_WINDOW:
		return printMirror(target);

	case DWM_THUMBNAIL:
		return dwmThumbnailMirror(target);
	}

	return NULL;
}

void adjustThumbnailPosition(int leftOffset, int rightOffset, int topOffset, int bottomOffset, boolean isResizingWindow) {
	RECT* rectSource = &dskThumbProps.rcSource;

	if (rectSource->left + leftOffset < 0)
	{
		rectSource->left = 0;
	}else rectSource->left += leftOffset;

	if (rectSource->right + rightOffset < 0)
	{
		rectSource->right = 0;
	}else rectSource->right += rightOffset;

	if (rectSource->top + topOffset < 0)
	{
		rectSource->top = 0;
	}else rectSource->top += topOffset;
	
	if (rectSource->bottom + bottomOffset < 0)
	{
		rectSource->bottom = 0;
	}else rectSource->bottom += bottomOffset;

	if (isResizingWindow)
	{
		RECT* rectDestination = &dskThumbProps.rcDestination;

		rectDestination->left += leftOffset;
		rectDestination->right += rightOffset;
		rectDestination->top += topOffset;
		rectDestination->bottom += bottomOffset;
	}
}

void setThumbnailPosition(int left, int right, int top, int bottom) {
	RECT* rectSource = &dskThumbProps.rcSource;

	int width = rectSource->right - rectSource->left;
	int height = rectSource->bottom - rectSource->top;

	if (left >= 0)
	{
		rectSource->left = left;
	}
	if (right >= 0)
	{
		rectSource->right = width + right;
	}
	if (top >= 0)
	{
		rectSource->top = top;
	}
	if (bottom >= 0)
	{
		rectSource->bottom = height + bottom;
	}

}

