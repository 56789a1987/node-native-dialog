#pragma once
#include "utils.h"
#include "resource.h"

VOID showProgressBox(INT argc, CHAR* argv[]);

typedef struct {
	HWND hwnd;
	BOOL finished;
	BOOL indeterminate;
	BOOL autoClose;
	BOOL noCancel;
	BYTE value;
	BOOL hasTaskbar;
	ITaskbarList3* taskbar;
} PROGRESS_BOX_STATE;
