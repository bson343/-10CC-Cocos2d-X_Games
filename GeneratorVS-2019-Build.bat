@echo off

set BuildDirectoryName="Win32-Build"

@echo:
@echo [Project Generator]
@echo -------------------------------------------------
PUSHD %~dp0
@echo Removing previous project folder.
IF EXIST %BuildDirectoryName% rd %BuildDirectoryName% /s /q
@echo:
@echo Done!
@echo:
@echo -------------------------------------------------
IF NOT EXIST %BuildDirectoryName% @echo Creating a new project folder.
IF NOT EXIST %BuildDirectoryName% md %BuildDirectoryName%
IF NOT EXIST %BuildDirectoryName% @echo:
cd %BuildDirectoryName%
@echo:
@echo Done!
@echo:
@echo -------------------------------------------------
@echo Running CMAKE script.
@echo:

cmake .. -G"Visual Studio 16 2019" -Tv140 -AWin32

@echo:
@echo Done!
@echo:
@echo -------------------------------------------------
@echo A new solution file is generated in %~dp0Project
@echo:
POPD
PAUSE