#include "inputBox.h"

VOID localizeWindow(HWND hwnd) {
	HMODULE hMUser32 = LoadLibraryExW(L"user32.dll", NULL, LOAD_LIBRARY_AS_DATAFILE);
	if (hMUser32) {
		WCHAR tOK[64], tCancel[64];
		LoadStringW(hMUser32, 800, tOK, _countof(tOK));
		LoadStringW(hMUser32, 801, tCancel, _countof(tCancel));
		FreeLibrary(hMUser32);
		SetWindowTextW(GetDlgItem(hwnd, IDOK), tOK);
		SetWindowTextW(GetDlgItem(hwnd, IDCANCEL), tCancel);
	}
}

VOID getResult(HWND hwnd) {
	WCHAR wResult[1024];
	HWND inputEdit = GetDlgItem(hwnd, IDC_INPUT_BOX_EDIT);
	GetWindowTextW(inputEdit, wResult, GetWindowTextLengthW(inputEdit) + 1);
	wprintf(L"%s", wResult);
}

INT_PTR CALLBACK inputBoxProc(HWND hwnd, UINT message, WPARAM wParam, LPARAM lParam) {
	switch (message) {
	case WM_INITDIALOG:
		return TRUE;
	case WM_DESTROY:
		PostQuitMessage(0);
		return TRUE;
	case WM_CLOSE:
		setExitCode(1);
		DestroyWindow(hwnd);
		return TRUE;
	case WM_COMMAND:
		switch (LOWORD(wParam)) {
		case IDOK: {
			getResult(hwnd);
			DestroyWindow(hwnd);
			return TRUE;
		}
		case IDCANCEL: {
			setExitCode(1);
			DestroyWindow(hwnd);
			return TRUE;
		}
		}
	}
	return FALSE;
}

VOID showInputBox(CHAR* text, CHAR* title, CHAR* initial, BOOL password) {
	WCHAR wTitle[256], wInitial[256], wText[1024];
	toWideChar(wText, text);
	toWideChar(wTitle, title);
	toWideChar(wInitial, initial);

	HWND inputBox = CreateDialogW(GetModuleHandleW(NULL), MAKEINTRESOURCEW(IDD_INPUTBOX), NULL, inputBoxProc);
	HWND inputEdit = GetDlgItem(inputBox, IDC_INPUT_BOX_EDIT);

	SetWindowTextW(inputBox, wTitle);
	SetWindowTextW(GetDlgItem(inputBox, IDC_INPUT_BOX_LABEL), wText);
	SetWindowTextW(inputEdit, wInitial);
	localizeWindow(inputBox);

	if (password) {
		SendMessageW(inputEdit, EM_SETPASSWORDCHAR, '*', 0);
	}

	ShowWindow(inputBox, SW_SHOW);
	SetFocus(inputEdit);
	SendMessageW(inputEdit, EM_SETSEL, 0, -1);

	MSG message;
	while (GetMessageW(&message, NULL, 0, 0)) {
		if (!IsDialogMessageW(inputBox, &message)) {
			TranslateMessage(&message);
			DispatchMessageW(&message);
		}
	}
}
