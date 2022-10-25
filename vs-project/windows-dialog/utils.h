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

BOOL createTaskbarInstance(ITaskbarList3** taskbar);

typedef HRESULT(WINAPI* f_CoCreateInstance)
	(REFCLSID rclsid, LPUNKNOWN pUnkOuter, DWORD dwClsContext, REFIID riid, LPVOID* ppv);
