@ECHO OFF
SETLOCAL

SET PROG_ACC="./Tools/ACC/acc.EXE"
SET ACSSOURCE="./ACS Source"
SET ACS_DEST="./PK3 Source/acs"

RMDIR /S /Q "./PK3 Source/acs"
MKDIR "./PK3 Source/acs"
%PROG_ACC% %ACSSOURCE%/Se7evidas.ACS %ACS_DEST%/Se7evidas.O

PAUSE