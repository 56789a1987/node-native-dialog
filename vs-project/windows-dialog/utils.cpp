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
