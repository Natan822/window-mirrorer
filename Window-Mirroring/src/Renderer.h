#pragma once

extern DWORD WINAPI mirrorWindow(LPVOID lParam);

extern void adjustThumbnailPosition(int leftOffset, int rightOffset, int topOffset, int bottomOffset);
extern void setThumbnailPosition(int left, int right, int top, int bottom);

enum MirrorType {
	BITBLT,
	PRINT_WINDOW,
	DWM_THUMBNAIL
};