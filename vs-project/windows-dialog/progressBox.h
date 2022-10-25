#pragma once
#include "utils.h"

VOID showProgressBox(INT argc, CHAR* argv[]);

typedef struct {
	BOOL* ended;
	DWORD* progress;
	IProgressDialog* dialog;
} PROGRESS_BOX_STATE;
