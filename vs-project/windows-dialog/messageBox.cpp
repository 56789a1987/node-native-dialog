#include "messageBox.h"

VOID showInfoMessage(CHAR* text, CHAR* title) {
	WCHAR wTitle[256], wText[1024];
	toWideChar(wText, text);
	toWideChar(wTitle, title);

	MessageBoxW(NULL, wText, wTitle, MB_ICONASTERISK | MB_OK);
}

VOID showErrorMessage(CHAR* text, CHAR* title) {
	WCHAR wTitle[256], wText[1024];
	toWideChar(wText, text);
	toWideChar(wTitle, title);

	MessageBoxW(NULL, wText, wTitle, MB_ICONERROR | MB_OK);
}

VOID showWarningMessage(CHAR* text, CHAR* title) {
	WCHAR wTitle[256], wText[1024];
	toWideChar(wText, text);
	toWideChar(wTitle, title);

	MessageBoxW(NULL, wText, wTitle, MB_ICONEXCLAMATION | MB_OK);
}

VOID showQuestionMessage(CHAR* text, CHAR* title) {
	WCHAR wTitle[256], wText[1024];
	toWideChar(wText, text);
	toWideChar(wTitle, title);

	INT result = MessageBoxW(NULL, wText, wTitle, MB_ICONQUESTION | MB_OKCANCEL);
	if (result != IDOK) {
		setExitCode(1);
	}
}
