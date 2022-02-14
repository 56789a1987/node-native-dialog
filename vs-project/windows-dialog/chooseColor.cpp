#include "chooseColor.h"

VOID showChooseColor(INT argc, CHAR* argv[]) {
	CHOOSECOLORW cc;
	DWORD color = 0x000000;
	COLORREF customColors[16];

	ZeroMemory(&cc, sizeof(cc));
	ZeroMemory(&customColors, sizeof(customColors));

	cc.Flags = CC_RGBINIT;

	for (INT i = 2; i < argc; i++) {
		CHAR* arg = argv[i];
		if (arg[0] != '-')
			continue;

		switch (arg[1]) {
		case 'c':
			color = atoi(argv[++i]);
			break;
		case 'f':
			cc.Flags |= CC_FULLOPEN;
			break;
		case 't':
			for (INT j = 0; j < 16; j++) {
				customColors[j] = atoi(argv[++i]);
			}
			break;
		}
	}

	cc.lStructSize = sizeof(cc);
	cc.lpCustColors = customColors;
	cc.rgbResult = color;

	if (ChooseColorW(&cc)) {
		wprintf(L"%u", cc.rgbResult);
	}
	else {
		setExitCode(1);
	}
}
