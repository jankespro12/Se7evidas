@ECHO OFF
SETLOCAL

REM Shops Stuff
ECHO Compiling shops
MAKE all

REM 7z stuff
SET MAIN_7Z=%CD%
SET PROG_7Z="%CD%/Tools/7z/7z"
SET PK3SOURCE="%CD%/PK3 Source"

IF EXIST "%MAIN_7Z%/Se7evidas/" (
    RMDIR Se7evidas /S /Q
)
IF EXIST "%MAIN_7Z%/Se7evidas.PK3" (
    DEL "%MAIN_7Z%/Se7evidas.PK3"
)

ECHO Creating temporary directories
MKDIR Se7evidas
MKDIR Se7evidas\README
MKDIR Se7evidas\Se7evidas-Licenses

ECHO Compiling Se7evidas.PK3
CD %PK3SOURCE%
%PROG_7Z% a -tzip "%MAIN_7Z%/Se7evidas.PK3" "*" -mx0 -r -x!*.DB -X!*.DBS -X!*.WAD.BACKUP* -X!*.TMP*
CD %MAIN_7Z%

DEL Se7evidas.7Z /Q
ECHO Creating Se7evidas-Licenses.ZIP
ECHO F | XCOPY LICENSE "%MAIN_7Z%/Se7evidas/Se7evidas-Licenses/Assets.txt" /Q /Y
%PROG_7Z% A -t7z "%MAIN_7Z%/Se7evidas/README/Se7evidas-Licenses.ZIP" -m0=Deflate -mx0 -ms=off "%MAIN_7Z%/Se7evidas/Se7evidas-Licenses/*"
RMDIR Se7evidas\Se7evidas-Licenses /S /Q

ECHO Creating Se7evidas-README.ZIP
XCOPY Se7evidas-Bugs.TXT "%MAIN_7Z%/Se7evidas/README"
XCOPY Se7evidas-TODO.TXT "%MAIN_7Z%/Se7evidas/README"
XCOPY Se7evidas-README.TXT "%MAIN_7Z%/Se7evidas/README"
%PROG_7Z% A -t7z "%MAIN_7Z%/Se7evidas/Se7evidas-README.ZIP" -m0=Deflate -mx0 -ms=off "%MAIN_7Z%/Se7evidas/README/*"
RMDIR Se7evidas\README /S /Q

ECHO Creating Se7evidas.7Z
XCOPY Se7evidas.PK3 "%MAIN_7Z%/Se7evidas" /Q /Y
%PROG_7Z% A -t7z "%MAIN_7Z%/Se7evidas.7Z" -m0=LZMA2 -mx9 -ms=off "%MAIN_7Z%/Se7evidas/*"

ECHO Erasing temporary files and directories
RMDIR Se7evidas /S /Q
DEL "%MAIN_7Z%\Se7evidas.PK3" /Q