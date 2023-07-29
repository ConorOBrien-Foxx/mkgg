@ECHO off
ECHO Compiling src...
gcc src/mkgg.c -Wall -g -o mkgg

ECHO Compiling lib...
CD /d "lib"
FOR %%i IN (*.c) DO (
    ECHO   [.] Compiling lib/%%i...
    gcc "%%i" -o "%%~ni.dll" -Wall -g -shared
    IF ERRORLEVEL 1 (
        ECHO   [X] Compilation failed for %%i
    ) ELSE (
        ECHO   [+] Compiled to lib/%%~ni.dll!
    )
    REM ECHO.
)
CD /d ..
ECHO Done compiling.
