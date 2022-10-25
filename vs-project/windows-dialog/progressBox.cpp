#include "progressBox.h"

DWORD WINAPI listenerThread(LPVOID lparam) {
	PROGRESS_BOX_STATE* state = (PROGRESS_BOX_STATE*)lparam;
	WCHAR line[1024];

	while (!(*state->ended)) {
		fgetws(line, _countof(line), stdin);
		if (!line) continue;

		CONST WCHAR chr = line[0];
		if (chr == '#' && line[1] == ' ') {
			state->dialog->SetLine(1, line + 2, TRUE, NULL);
		}
		else if (chr >= '0' && chr <= '9') {
			*state->progress = _wtoi(line);
			state->dialog->SetProgress(*state->progress, 100u);
		}
	}

	return 0;
}

VOID showProgressBox(INT argc, CHAR* argv[]) {
	IProgressDialog *dialog;
	DWORD threadId;
	DWORD dwFlags = PROGDLG_NORMAL | PROGDLG_MODAL;
	DWORD progress = 0;
	BOOL ended = FALSE;

	if (FAILED(CoInitialize(NULL))) {
		MessageBoxW(NULL, L"Failed to show progress dialog", L"Error", MB_OK | MB_ICONHAND);
		return;
	}
	
	if (FAILED(CoCreateInstance(CLSID_ProgressDialog, NULL, CLSCTX_ALL, IID_IProgressDialog, (LPVOID*)&dialog))) {
		MessageBoxW(NULL, L"Failed to show progress dialog", L"Error", MB_OK | MB_ICONHAND);
		return;
	}

	// parse arguments
	for (INT i = 2; i < argc; i++) {
		CHAR* arg = argv[i];
		if (arg[0] != '-')
			continue;

		switch (arg[1]) {
		case 't': {
			WCHAR wText[256];
			toWideChar(wText, argv[++i]);
			dialog->SetTitle(wText);
			break;
		}
		case 'm': {
			WCHAR wText[1024];
			toWideChar(wText, argv[++i]);
			dialog->SetLine(1, wText, TRUE, NULL);
			break;
		}
		case 'p':
			progress = atoi(argv[++i]);
			break;
		case 'i':
			dwFlags |= PROGDLG_MARQUEEPROGRESS;
			break;
		case 'n':
			dwFlags |= PROGDLG_NOCANCEL;
			break;
		}
	}

	if (FAILED(dialog->StartProgressDialog(NULL, NULL, dwFlags, NULL))) {
		MessageBoxW(NULL, L"Failed to show progress dialog", L"Error", MB_OK | MB_ICONHAND);
		return;
	}

	dialog->SetProgress(progress, 100u);

	PROGRESS_BOX_STATE state = { &ended, &progress, dialog };
	CreateThread(NULL, 0, listenerThread, &state, 0, &threadId);

	while (!dialog->HasUserCancelled() && progress != 100) {
		Sleep(10u);
	}

	dialog->StopProgressDialog();
	ended = TRUE;

	if (dialog->HasUserCancelled()) {
		setExitCode(1);
	}
}
