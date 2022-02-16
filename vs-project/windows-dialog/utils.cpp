#include "utils.h"

VOID toWideChar(WCHAR* result, CONST CHAR* source) {
	mbstowcs(result, source, strlen(source) + 1);
}

INT exitCode = 0;

INT getExitCode() {
	return exitCode;
}

VOID setExitCode(INT code) {
	exitCode = code;
}

// localize OK and Cancel buttons
VOID localizeWindow(HWND hwnd) {
	HMODULE hMUser32 = LoadLibraryExW(L"user32.dll", NULL, LOAD_LIBRARY_AS_DATAFILE);
	if (hMUser32) {
		WCHAR tOK[64], tCancel[64];
		LoadStringW(hMUser32, 800, tOK, _countof(tOK));
		LoadStringW(hMUser32, 801, tCancel, _countof(tCancel));
		FreeLibrary(hMUser32);
		SetWindowTextW(GetDlgItem(hwnd, IDOK), tOK);
		SetWindowTextW(GetDlgItem(hwnd, IDCANCEL), tCancel);
		FreeLibrary(hMUser32);
	}
}

// taskbar API for MinGW build and old OS
BOOL createTaskbarInstance(ITaskbarList3** taskbar) {
	BOOL succeeded = FALSE;
	HMODULE hMOle32 = LoadLibraryW(L"Ole32.dll");
	if (hMOle32) {
		f_CoCreateInstance CoCreateInstance = (f_CoCreateInstance)GetProcAddress(hMOle32, "CoCreateInstance");
		if (SUCCEEDED(CoCreateInstance(CLSID_TaskbarList, NULL, CLSCTX_ALL, IID_ITaskbarList3, (LPVOID*)taskbar))) {
			if (SUCCEEDED((*taskbar)->HrInit()))
				succeeded = TRUE;
			else
				(*taskbar)->Release();
		}
		FreeLibrary(hMOle32);
	}
	return succeeded;
}
