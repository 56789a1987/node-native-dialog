windres windows-dialog\windows-dialog.rc -O coff -o windows-dialog.res && ^
i686-w64-mingw32-g++ windows-dialog\*.cpp windows-dialog.res -mwindows -mconsole -o dialog.exe && ^
dialog 0 "Build & test success" "Info"
