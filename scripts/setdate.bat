@echo off
:loop
date 06-17-15
echo Date set to 06-17-15.
date /T
echo next date reset in 1 hour.
sleep 3600
goto loop
