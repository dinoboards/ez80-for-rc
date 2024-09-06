@echo off

SETLOCAL

if "%CUPLPATH%" == "" set CUPLPATH=z:\Wincupl\Shared

set PLD_SRC=%1

if "%PLD_SRC%" == "" set PLD_SRC=ez80-cpu-v1.7

if not exist "bin" mkdir bin

echo Compiling %PLD_SRC%.jed from %PLD_SRC%.pld
echo.

%CUPLPATH%\cupl.exe -e -x -f - -ju %CUPLPATH%\cupl.dl %PLD_SRC%.pld
if %errorlevel% neq 0 exit /b %errorlevel%

REM Removed the location entry - the TL866II Plus programmer seems to have loading file issue

findstr /v /i /L /c:"Location" %PLD_SRC%.jed >bin\%PLD_SRC%.jed

move %PLD_SRC%.doc bin\%PLD_SRC%.doc
move %PLD_SRC%.mx bin\%PLD_SRC%.mx

del %PLD_SRC%.jed

echo.
