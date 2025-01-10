@echo off
SETLOCAL

cd release
make -B -f .\ez80-for-rc-firmware_release.mak

copy ..\bin\ez80-rc-firmware-release.hex ..\..\programmer\

REM Get the current date in YYYY-MM-DD format
for /f "tokens=*" %%i in ('wmic os get localdatetime /value') do set datetime=%%i
set year=%datetime:~0,4%
set month=%datetime:~4,2%
set day=%datetime:~6,2%
set formattedDate=%year%-%month%-%day%

REM Define the source and destination files
set sourceFile=..\src\version.inc
set destinationFile=..\..\programmer\firmware-version.inc

REM Copy the file with the new name
copy %sourceFile% %destinationFile%

REM Append the current date to the destination file
echo "Firmware build date recorded as: " %formattedDate% " into file " %destinationFile%
echo FIRMWARE_DATE  .EQU  "%formattedDate%" >> %destinationFile%

