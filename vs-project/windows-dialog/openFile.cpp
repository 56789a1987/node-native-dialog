#include "openFile.h"

VOID showOpenFile(CONST BOOL save, INT argc, CHAR* argv[]) {
	OPENFILENAMEW ofn;
	WCHAR output[1024] = { 0 };
	BOOL status, multiple = FALSE;
	WCHAR title[256], filters[1024], initial[256];

	ZeroMemory(&ofn, sizeof(OPENFILENAMEW));

	ofn.lStructSize = sizeof(OPENFILENAMEW);
	ofn.lpstrFilter = L"All files (*.*)\0*.*\0";

	for (INT i = 2; i < argc; i++) {
		CHAR* arg = argv[i];
		if (arg[0] != '-')
			continue;

		switch (arg[1]) {
		case 'm':
			multiple = TRUE;
			break;
		case 'n':
			toWideChar(output, argv[++i]);
			break;
		case 't':
			toWideChar(title, argv[++i]);
			ofn.lpstrTitle = title;
			break;
		case 'i':
			toWideChar(initial, argv[++i]);
			ofn.lpstrInitialDir = initial;
			break;
		case 'f':
			toWideChar(filters, argv[++i]);
			ofn.lpstrFilter = filters;
			for (INT i = 0; i < sizeof(filters); i++) {
				if (filters[i] == '|') filters[i] = '\0';
			}
			break;
		}
	}

	ofn.lpstrFile = output;
	ofn.nMaxFile = 1024;
	ofn.nFilterIndex = 0;
	ofn.Flags = OFN_EXPLORER | OFN_PATHMUSTEXIST;

	if (save) {
		ofn.Flags |= OFN_OVERWRITEPROMPT;
		status = GetSaveFileNameW(&ofn);
	}
	else {
		ofn.Flags |= OFN_FILEMUSTEXIST;
		if (multiple) ofn.Flags |= OFN_ALLOWMULTISELECT;
		status = GetOpenFileNameW(&ofn);
	}

	if (status) {
		WCHAR* ptr = ofn.lpstrFile;
		LPWSTR dir = ptr;
		wprintf(L"%s", dir);
		ptr += lstrlenW(dir) + 1;

		while (*ptr) {
			LPWSTR file = ptr;
			wprintf(L"\n%s", file);
			ptr += lstrlenW(file) + 1;
		}
	}
	else {
		setExitCode(1);
	}
}
