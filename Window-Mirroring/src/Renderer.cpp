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

int initialRcBottom = 0;
int initialRcRight = 0;

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
	rcSource.left = 0; // Start from the middle
	rcSource.top = 0;
	rcSource.right = mainRect.right - mainRect.left - SCROLLBAR_SIZE;
	rcSource.bottom = mainRect.bottom - mainRect.top - (SCROLLBAR_SIZE * 1.8);

	initialRcRight = rcSource.right;
	initialRcBottom = rcSource.bottom;

	// Define the destination rectangle where the thumbnail will be displayed
	RECT rcDestination;
	rcDestination.left = 0;
	rcDestination.top = 0;
	rcDestination.right = rcSource.right;
	rcDestination.bottom = rcSource.bottom;
	//rcDestination.right = (rcSource.right - rcSource.left); // Adjust as needed
	//rcDestination.bottom = (rcSource.bottom - rcSource.top); // Adjust as needed

	dskThumbProps.dwFlags = DWM_TNP_RECTSOURCE | DWM_TNP_RECTDESTINATION | DWM_TNP_VISIBLE | DWM_TNP_SOURCECLIENTAREAONLY;

	dskThumbProps.fSourceClientAreaOnly = TRUE;
	dskThumbProps.fVisible = TRUE;
	dskThumbProps.rcDestination = rcDestination;
	dskThumbProps.rcSource = rcSource;
	//dskThumbProps.opacity = (255 * 70) / 100;


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

void adjustThumbnailPosition(int leftOffset, int rightOffset, int topOffset, int bottomOffset) {
	RECT* rect = &dskThumbProps.rcSource;

	if (rect->left + leftOffset < 0)
	{
		rect->left = 0;
	}else rect->left += leftOffset;

	if (rect->right + rightOffset - initialRcRight < 0)
	{
		rect->right = 0;
	}else rect->right += rightOffset;

	if (rect->top + topOffset < 0)
	{
		rect->top = 0;
	}else rect->top += topOffset;
	
	if (rect->bottom + bottomOffset - initialRcBottom < 0)
	{
		rect->bottom = 0;
	}else rect->bottom += bottomOffset;
}

void setThumbnailPosition(int left, int right, int top, int bottom) {
	RECT* rect = &dskThumbProps.rcSource;

	if (left >= 0)
	{
		rect->left = left;
	}
	if (right >= 0)
	{
		rect->right = initialRcRight + right;
	}
	if (top >= 0)
	{
		rect->top = top;
	}
	if (bottom >= 0)
	{
		rect->bottom = initialRcBottom + bottom;
	}
}