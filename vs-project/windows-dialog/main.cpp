#include "utils.h"
#include "messageBox.h"
#include "chooseColor.h"
#include "openFile.h"
#include "browseFolder.h"
#include "inputBox.h"

enum Type {
	TYPE_INFO = '0',
	TYPE_ERROR,
	TYPE_WARNING,
	TYPE_QUESTION,
	TYPE_ENTRY,
	TYPE_PASSWORD,
	TYPE_COLOR,
	TYPE_OPEN,
	TYPE_SAVE,
	TYPE_DIRECTORY,
};

INT main(INT argc, CHAR* argv[]) {
	setlocale(LC_CTYPE, "");
	CHAR* type = argv[1];
	if (!type) {
		return 0;
	}

	switch (type[0]) {
	case TYPE_INFO:
		showInfoMessage(argv[2], argv[3]);
		break;
	case TYPE_ERROR:
		showErrorMessage(argv[2], argv[3]);
		break;
	case TYPE_WARNING:
		showWarningMessage(argv[2], argv[3]);
		break;
	case TYPE_QUESTION:
		showQuestionMessage(argv[2], argv[3]);
		break;
	case TYPE_ENTRY:
		showInputBox(argv[2], argv[3], argv[4], FALSE);
		break;
	case TYPE_PASSWORD:
		showInputBox(argv[2], argv[3], argv[4], TRUE);
		break;
	case TYPE_COLOR:
		showChooseColor(argc, argv);
		break;
	case TYPE_OPEN:
		showOpenFile(FALSE, argc, argv);
		break;
	case TYPE_SAVE:
		showOpenFile(TRUE, argc, argv);
		break;
	case TYPE_DIRECTORY:
		showBrowseFolder(argc, argv);
		break;
	}

	return getExitCode();
}
