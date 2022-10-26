#include "notification.h"

UINT const WMAPP_NOTIFYCALLBACK = WM_APP + 1;
UINT WM_TASKBAR_CREATED;
NOTIFYICONDATAW nid;

VOID registerWindowClass(HINSTANCE hInstance, PCWSTR pszClassName, WNDPROC lpfnWndProc) {
	WNDCLASSEX wcex;
	ZeroMemory(&wcex, sizeof(wcex));
	wcex.cbSize = sizeof(wcex);
	wcex.style = CS_HREDRAW | CS_VREDRAW;
	wcex.lpfnWndProc = lpfnWndProc;
	wcex.hInstance = hInstance;
	wcex.lpszClassName = pszClassName;
	RegisterClassEx(&wcex);
}

LRESULT CALLBACK notifyWndProc(HWND hwnd, UINT message, WPARAM wParam, LPARAM lParam) {
	// Add the icon at taskbar create, in case the icon disappears if Explorer restarts
	if (message == WM_TASKBAR_CREATED) {
		Shell_NotifyIconW(NIM_ADD, &nid);
		return TRUE;
	}

	switch (message) {
	case WM_CREATE:
		// Add the icon when the window is created
		nid.hWnd = hwnd;
		Shell_NotifyIconW(NIM_ADD, &nid);
		return TRUE;
	case WM_DESTROY:
		// Remove the icon and exit when the window is destroyed
		Shell_NotifyIconW(NIM_DELETE, &nid);
		PostQuitMessage(0);
		return TRUE;
	case WMAPP_NOTIFYCALLBACK:
		// If the balloon is clicked or timeouts, also destroy the window
		switch (LOWORD(lParam)) {
		case NIN_SELECT: case NIN_BALLOONUSERCLICK: case NIN_BALLOONTIMEOUT:
			DestroyWindow(hwnd);
		}
		return TRUE;
	default:
		return DefWindowProcW(hwnd, message, wParam, lParam);
	}
}

VOID showNotification(INT argc, CHAR* argv[]) {
	HINSTANCE hInstance = GetModuleHandleW(NULL);
	WCHAR className[] = L"NotificationHost";
	WM_TASKBAR_CREATED = RegisterWindowMessageW(L"TaskbarCreated");
	registerWindowClass(hInstance, className, notifyWndProc);

	ZeroMemory(&nid, sizeof(nid));
	nid.cbSize = sizeof(nid);
	nid.uFlags = NIF_ICON | NIF_INFO | NIF_MESSAGE;
	nid.uCallbackMessage = WMAPP_NOTIFYCALLBACK;
	nid.hIcon = LoadIconW(NULL, IDI_APPLICATION);

	// Parse args
	for (INT i = 2; i < argc; i++) {
		CHAR* arg = argv[i];
		if (arg[0] != '-')
			continue;

		switch (arg[1]) {
		case 't':
			toWideChar(nid.szInfoTitle, argv[++i]);
			break;
		case 'm':
			toWideChar(nid.szInfo, argv[++i]);
			break;
		case 'i':
			nid.dwInfoFlags = NIIF_INFO;
			break;
		case 'w':
			nid.dwInfoFlags = NIIF_WARNING;
			break;
		case 'e':
			nid.dwInfoFlags = NIIF_ERROR;
			break;
		}
	}

	// Notify icon needs a window, even if we don't need to show any window
	HWND hwnd = CreateWindowW(className, L"", NULL, 0, 0, 0, 0, NULL, NULL, hInstance, NULL);

	MSG message;
	while (GetMessageW(&message, NULL, 0, 0)) {
		TranslateMessage(&message);
		DispatchMessageW(&message);
	}
}
