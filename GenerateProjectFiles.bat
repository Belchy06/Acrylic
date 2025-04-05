@echo off
set IDE=%1
if [%IDE%]==[] goto usage
call Engine\Build\Premake\Win\premake5.exe %IDE%
goto :eof
:usage
@echo Usage: %0 "vscode|vs2022"
PAUSE