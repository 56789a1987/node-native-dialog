#pragma once
#pragma warning(disable : 4996)

#include <stdio.h>
#include <windows.h>
#include <commdlg.h>
#include <initguid.h>
#include <ShlGuid.h>
#include <ShlObj.h>
#include <locale.h>

VOID toWideChar(WCHAR* result, CONST CHAR* source);

INT getExitCode();
VOID setExitCode(INT code);

VOID localizeWindow(HWND hwnd);
