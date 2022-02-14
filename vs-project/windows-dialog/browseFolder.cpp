#include "browseFolder.h"

VOID showBrowseFolder(INT argc, CHAR* argv[]) {
	BROWSEINFOW bi;
	WCHAR title[256], folder[MAX_PATH];

	ZeroMemory(&bi, sizeof(bi));
	bi.lpszTitle = L"Select folder";
	
	for (INT i = 2; i < argc; i++) {
		CHAR* arg = argv[i];
		if (arg[0] != '-')
			continue;

		switch (arg[1]) {
		case 't':
			toWideChar(title, argv[++i]);
			bi.lpszTitle = title;
			break;
		case 'i':
			toWideChar(folder, argv[++i]);
			bi.lParam = (LPARAM)folder;
			break;
		}
	}

	bi.ulFlags = BIF_RETURNONLYFSDIRS | BIF_USENEWUI;

	LPITEMIDLIST pidl = SHBrowseForFolderW(&bi);

	if (pidl) {
		WCHAR result[MAX_PATH];
		if (SHGetPathFromIDListW(pidl, result)) {
			wprintf(L"%s", result);
		}
	}
	else {
		setExitCode(1);
	}
}
