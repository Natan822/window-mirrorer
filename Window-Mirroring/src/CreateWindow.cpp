#include "CreateWindow.h"
#include "global.h"
#include <iostream>

const int SCROLL_OFFSET = 10;
const int HORZ_SCROLL_SIZE = 58;
const int VERT_SCROLL_SIZE = 58;

void handleVerticalScrolling(HWND hWnd, WPARAM wParam) {
    SCROLLINFO scrollInfo = {};
    scrollInfo.cbSize = sizeof(SCROLLINFO);
    scrollInfo.fMask = SIF_ALL;

    ::GetScrollInfo(hWnd, SB_VERT, &scrollInfo);
    int previousYPos = scrollInfo.nPos;

    // User's scrolling request
    switch (LOWORD(wParam))
    {
    // Scrolls up by one unit
    case SB_LINEUP:
        if (scrollInfo.nPos == 0) 
            break;

        scrollInfo.nPos -= SCROLL_OFFSET;
        if (scrollInfo.nPos < 0)
        {
            scrollInfo.nPos = 0;
            setThumbnailPosition(-1, -1, 0, 0);
            break;
        }

        if (mirrorType == DWM_THUMBNAIL)
        {
            adjustThumbnailPosition(0, 0, (-SCROLL_OFFSET * 10), (-SCROLL_OFFSET * 10), false);
        }
        else
        {
            windowY -= (SCROLL_OFFSET * 10);
        }
        break;

        // Scrolls down by one unit
    case SB_LINEDOWN:
        scrollInfo.nPos += SCROLL_OFFSET;
        if (mirrorType == DWM_THUMBNAIL)
        {
            adjustThumbnailPosition(0, 0, (SCROLL_OFFSET * 10), (SCROLL_OFFSET * 10), false);
        }
        else
        {
            windowY += (SCROLL_OFFSET * 10);
        }
        break;

        // User is dragging the scrollbox
    case SB_THUMBTRACK:
        if (scrollInfo.nPos == HIWORD(wParam)) {
            break;
        }

        ::GetWindowRect(mainWindowHandle, &mainRect);
        scrollInfo.nPos = HIWORD(wParam);
        if (mirrorType == DWM_THUMBNAIL)
        {
            setThumbnailPosition(-1, -1, (HIWORD(wParam) * 10), (HIWORD(wParam) * 10));
        }
        else
        {
            windowY = HIWORD(wParam) * 10;
        }
        break;
    }
    ::SetScrollInfo(hWnd, SB_VERT, &scrollInfo, FALSE);
    ::ScrollWindow(hWnd, (previousYPos - scrollInfo.nPos), 0, 0, 0);

    ::UpdateWindow(hWnd);
}

void handleHorizontalScrolling(HWND hWnd, WPARAM wParam)
{
    SCROLLINFO scrollInfo = {};
    scrollInfo.cbSize = sizeof(SCROLLINFO);
    scrollInfo.fMask = SIF_ALL;

    ::GetScrollInfo(hWnd, SB_HORZ, &scrollInfo);

    int previousXPos = scrollInfo.nPos;

    // User's scrolling request
    switch (LOWORD(wParam))
    {
        // Scrolls left by one unit
    case SB_LINELEFT:
        if (scrollInfo.nPos == 0)
            break;

        scrollInfo.nPos -= SCROLL_OFFSET;
        if (scrollInfo.nPos < 0)
        {
            int diff = scrollInfo.nPos;
            scrollInfo.nPos = 0;
            setThumbnailPosition(0, 0, -1, -1);
            break;
		}

		if (mirrorType == DWM_THUMBNAIL)
		{
			adjustThumbnailPosition((-SCROLL_OFFSET * 10), (-SCROLL_OFFSET * 10), 0, 0, false);
		}
		else
		{
			windowX -= (SCROLL_OFFSET * 10);
		}
        break;

        // Scrolls right by one unit
    case SB_LINERIGHT:
        scrollInfo.nPos += SCROLL_OFFSET;
        if (mirrorType == DWM_THUMBNAIL)
        {
            adjustThumbnailPosition((SCROLL_OFFSET * 10), (SCROLL_OFFSET * 10), 0, 0, false);
        }
        else
        {
            windowX += (SCROLL_OFFSET * 10);
        }
        break;

        // User is dragging the scrollbox
    case SB_THUMBTRACK:
        if (scrollInfo.nPos == HIWORD(wParam)) {
            break;
        }

        ::GetWindowRect(mainWindowHandle, &mainRect);
        scrollInfo.nPos = HIWORD(wParam);
        if (mirrorType == DWM_THUMBNAIL)
        {
            std::cout << (HIWORD(wParam) * 10) << std::endl;
            setThumbnailPosition((HIWORD(wParam) * 10), (HIWORD(wParam) * 10), -1, -1);
        }
        else {
            windowX = HIWORD(wParam) * 10;
        }
        break;
    }
    ::SetScrollInfo(hWnd, SB_HORZ, &scrollInfo, FALSE);
    ::ScrollWindow(hWnd, (previousXPos - scrollInfo.nPos), 0, 0, 0);

}

LRESULT CALLBACK WindowProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam) {

    switch (uMsg)
    {

    case WM_MOVE:
        ::GetWindowRect(mainWindowHandle, &mainRect);
        break;

    case WM_SIZING:
    {
        RECT previousRect = mainRect;
        ::GetWindowRect(mainWindowHandle, &mainRect);

        int diffLeft = mainRect.left - previousRect.left;
        int diffRight = mainRect.right - previousRect.right;
        int diffTop = mainRect.top- previousRect.top;
        int diffBottom = mainRect.bottom - previousRect.bottom;
        
        adjustThumbnailPosition(diffLeft, diffRight, diffTop, diffBottom, true);

        break;
    }
    case WM_SETCURSOR:
        ::SetCursor(LoadCursor(NULL, IDC_ARROW));
        break;

    // Vertical Scroll
    case WM_VSCROLL:
        handleVerticalScrolling(hWnd, wParam);
        break;

    // Horizontal Scroll
    case WM_HSCROLL:
        handleHorizontalScrolling(hWnd, wParam);
        break;

    case WM_CLOSE:
        if (CONSOLE_ENABLED) 
        {
            ::FreeConsole();
        }
        ::ExitProcess(1);
        break;

    default:
        break;
    }

	return DefWindowProc(hWnd, uMsg, wParam, lParam);
}

void adjustScrollBarsSize(HWND hWnd) {
    // Horizontal scrollbar
    SCROLLINFO horzScrollInfo = {};
    horzScrollInfo.cbSize = sizeof(SCROLLINFO);
    horzScrollInfo.fMask = SIF_ALL;

    ::GetScrollInfo(hWnd, SB_HORZ, &horzScrollInfo);
    horzScrollInfo.nPage = HORZ_SCROLL_SIZE;
    ::SetScrollInfo(hWnd, SB_HORZ, &horzScrollInfo, FALSE);

    // Vertical scrollbar
    SCROLLINFO vertScrollInfo = {};
    vertScrollInfo.cbSize = sizeof(SCROLLINFO);
    vertScrollInfo.fMask = SIF_ALL;

    ::GetScrollInfo(hWnd, SB_VERT, &vertScrollInfo);
    vertScrollInfo.nPage = VERT_SCROLL_SIZE;
    ::SetScrollInfo(hWnd, SB_VERT, &vertScrollInfo, FALSE);


    ::UpdateWindow(hWnd);
}

HWND CreateMainWindow(HINSTANCE hInstance) {

    const wchar_t CLASS_NAME[] = L"Window Class";

    WNDCLASS wc = {};
    wc.lpfnWndProc = WindowProc;
    wc.hInstance = hInstance;
    wc.lpszClassName = CLASS_NAME;

    ::RegisterClass(&wc);

    HWND hWnd = ::CreateWindowEx(
        0,                              // Optional window styles.
        CLASS_NAME,                     // Window class
        L"Window Mirrorer",             // Window title
        WS_OVERLAPPEDWINDOW | WS_HSCROLL | WS_VSCROLL,            // Window style

        // Size and Position
        CW_USEDEFAULT, CW_USEDEFAULT, CW_USEDEFAULT, CW_USEDEFAULT,

        NULL,            // Parent window    
        NULL,            // Menu
        hInstance,       // Instance handle
        NULL             // Additional application data
    );

    adjustScrollBarsSize(hWnd);

    // Make the window topmost
    ::SetWindowPos(hWnd, HWND_TOPMOST, 0, 0, 0, 0, SWP_NOMOVE | SWP_NOSIZE);

    return hWnd;
}