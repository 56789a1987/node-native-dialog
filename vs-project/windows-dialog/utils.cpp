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
