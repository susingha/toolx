@ECHO OFF

:LOOPSTART

time /T
ping 171.71.231.17  -t -n 2

:: Sleep for 15 minutes
timeout 900

GOTO LOOPSTART
