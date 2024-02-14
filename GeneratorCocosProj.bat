@echo off

set ProjectName=%1
set BuildDirectoryName="Win32-Build"
set OriginPath=%~p0


IF exist %ProjectName% (
    echo "A project that already exists."
    goto:EOF
)

echo "new cocos Project"

CALL cocos new -l cpp %ProjectName%

cd %ProjectName%

mkdir %BuildDirectoryName% && cd %BuildDirectoryName%

echo "Build CMake"

CALL cmake .. -G"Visual Studio 16 2019" -Tv140 -AWin32

cd %OriginPath%
