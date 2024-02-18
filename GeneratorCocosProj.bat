@echo off

set ProjectName=%1
set BuildDirectoryName="Win32-Build"
set OriginPath=%~p0
set BuildScriptName="GeneratorVS-2019-Build.bat"

PUSHD %~dp0

IF exist %ProjectName% (
    echo "A project that already exists."
    goto:EOF
)

echo "new cocos Project"

CALL cocos new -l cpp %ProjectName%

copy %~dp0\%BuildScriptName% %~dp0\%ProjectName%

cd %ProjectName%

CALL %BuildScriptName%

POPD


@REM cd %ProjectName%

@REM mkdir %BuildDirectoryName% && cd %BuildDirectoryName%

@REM echo "Build CMake"

@REM CALL cmake .. -G"Visual Studio 16 2019" -Tv140 -AWin32

@REM cd %OriginPath%
