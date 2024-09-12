#pragma once

extern DWORD WINAPI mirrorWindow(LPVOID lParam);

extern void updateThumbnailPosition(int leftOffset, int rightOffset, int topOffset, int bottomOffset);

enum MirrorType {
	BITBLT,
	PRINT_WINDOW,
	DWM_THUMBNAIL
};