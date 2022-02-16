#include "progressBox.h"

PROGRESS_BOX_STATE* state;

VOID updateControlStates(BYTE value, BOOL force) {
	HWND hwnd = state->hwnd;
	if (value == state->value && !force)
		return;

	// update the progress bar
	if (!state->indeterminate) {
		SendMessageW(GetDlgItem(hwnd, IDC_PROGRESS_BOX_PROGRESS), PBM_SETPOS, (WPARAM)value, 0);
		if (state->hasTaskbar)
			state->taskbar->SetProgressValue(hwnd, value, 100);
	}

	// enable / disable the OK button
	if (value >= 100) {
		if (state->value < 100 || force)
			EnableWindow(GetDlgItem(hwnd, IDOK), TRUE);
	}
	else if (state->value >= 100 || force) {
		EnableWindow(GetDlgItem(hwnd, IDOK), FALSE);
	}

	state->value = value;
}

VOID setFinished() {
	HWND hwnd = state->hwnd;
	HWND progress = GetDlgItem(hwnd, IDC_PROGRESS_BOX_PROGRESS);

	// enable the OK button, disable the Cancel button
	EnableWindow(GetDlgItem(hwnd, IDOK), TRUE);
	EnableWindow(GetDlgItem(hwnd, IDCANCEL), FALSE);

	// remove indeterminate state if necessary, and set progress to 100%
	if (state->indeterminate) {
		SetWindowLongW(progress, GWL_STYLE, GetWindowLongW(progress, GWL_STYLE) & ~PBS_MARQUEE);
		SendMessageW(progress, PBM_SETMARQUEE, FALSE, 0);
	}
	SendMessageW(progress, PBM_SETPOS, (WPARAM)100, 0);

	// taskbar progress
	if (state->hasTaskbar) {
		state->taskbar->SetProgressState(hwnd, TBPF_NORMAL);
		state->taskbar->SetProgressValue(hwnd, 100, 100);
	}

	// close the window
	if (state->autoClose) {
		PostMessageW(hwnd, WM_COMMAND, IDOK, 0);
	}

	state->finished = TRUE;
}

DWORD WINAPI listenerThread(LPVOID param) {
	WCHAR line[1024];
	HWND label = GetDlgItem(state->hwnd, IDC_PROGRESS_BOX_LABEL);

	while (!state->finished) {
		fgetws(line, _countof(line), stdin);
		if (!line) continue;

		CONST WCHAR chr = line[0];
		if (chr == '#' && line[1] == ' ') {
			SetWindowTextW(label, line + 2);
		}
		else if (chr >= '0' && chr <= '9') {
			updateControlStates(_wtoi(line), FALSE);
		}
		else if (chr == '-') {
			setFinished();
		}
	}

	return 0;
}

INT_PTR CALLBACK progressBoxProc(HWND hwnd, UINT message, WPARAM wParam, LPARAM lParam) {
	switch (message) {
	case WM_INITDIALOG:
		return TRUE;
	case WM_DESTROY:
		PostQuitMessage(0);
		return TRUE;
	case WM_CLOSE: {
		if (!state->noCancel || state->finished) {
			if (!state->finished)
				setExitCode(1);
			DestroyWindow(hwnd);
		}
		return TRUE;
	}
	case WM_COMMAND:
		switch (LOWORD(wParam)) {
		case IDOK:
			DestroyWindow(hwnd);
			return TRUE;
		case IDCANCEL:
			setExitCode(1);
			DestroyWindow(hwnd);
			return TRUE;
		}
	}
	return FALSE;
}

VOID setNoCancel(HWND hwnd) {
	// remove the close button
	SetWindowLongW(hwnd, GWL_STYLE, GetWindowLongW(hwnd, GWL_STYLE) & ~WS_SYSMENU);

	// move the OK button to where the Cancel button is, and hide the cancel button
	HWND cancel = GetDlgItem(hwnd, IDCANCEL);
	RECT rect;
	GetWindowRect(cancel, &rect);
	POINT point = { rect.left, rect.top };
	ScreenToClient(hwnd, &point);
	SetWindowPos(GetDlgItem(hwnd, IDOK), NULL, point.x, point.y, 0, 0, SWP_NOSIZE);
	ShowWindow(cancel, SW_HIDE);
}

VOID showProgressBox(INT argc, CHAR* argv[]) {
	DWORD threadId;
	HWND hwnd = CreateDialogW(GetModuleHandleW(NULL), MAKEINTRESOURCEW(IDD_PROGRESS_BOX), NULL, progressBoxProc);
	HWND progress = GetDlgItem(hwnd, IDC_PROGRESS_BOX_PROGRESS);

	ITaskbarList3* taskbar;
	BOOL hasTaskbar = createTaskbarInstance(&taskbar);
	PROGRESS_BOX_STATE _state = { hwnd, FALSE, FALSE, FALSE, FALSE, 0, hasTaskbar, taskbar };
	state = &_state;

	localizeWindow(hwnd);
	SendMessageW(progress, PBM_SETRANGE, 0, MAKELPARAM(0, 100));

	// parse arguments
	for (INT i = 2; i < argc; i++) {
		CHAR* arg = argv[i];
		if (arg[0] != '-')
			continue;

		switch (arg[1]) {
		case 't': {
			WCHAR wText[256];
			toWideChar(wText, argv[++i]);
			SetWindowTextW(hwnd, wText);
			break;
		}
		case 'm': {
			WCHAR wText[1024];
			toWideChar(wText, argv[++i]);
			SetWindowTextW(GetDlgItem(hwnd, IDC_PROGRESS_BOX_LABEL), wText);
			break;
		}
		case 'p':
			state->value = atoi(argv[++i]);
			break;
		case 'i':
			state->indeterminate = TRUE;
			break;
		case 'a':
			state->autoClose = TRUE;
			break;
		case 'n':
			state->noCancel = TRUE;
			setNoCancel(hwnd);
			break;
		}
	}

	ShowWindow(hwnd, SW_SHOW);
	updateControlStates(state->value, TRUE);
	CreateThread(NULL, 0, listenerThread, NULL, 0, &threadId);

	// set progress bar to indeterminate state
	if (state->indeterminate) {
		SetWindowLongW(progress, GWL_STYLE, GetWindowLongW(progress, GWL_STYLE) | PBS_MARQUEE);
		SendMessageW(progress, PBM_SETMARQUEE, TRUE, 0);
		if (hasTaskbar) taskbar->SetProgressState(hwnd, TBPF_INDETERMINATE);
	}

	MSG message;
	while (GetMessageW(&message, NULL, 0, 0)) {
		if (!IsDialogMessageW(hwnd, &message)) {
			TranslateMessage(&message);
			DispatchMessageW(&message);
		}
	}
}
