@echo off
REM https://blogs.yahoo.co.jp/cn051110/34213309.html
setlocal ENABLEDELAYEDEXPANSION

set PATHLF=!cd:\=^

!

for /F "delims=\" %%a in ("!PATHLF!") do (set cDIR=%%a)

endlocal && set cDIR=%cDIR%

set NEWDIR=..\cmakebuild\%cDIR%

REM https://tech.la-fra.com/2019/01/24/post-484/
If not exist %NEWDIR% mkdir %NEWDIR%

cmake -S . -B %NEWDIR% -G"Visual Studio 15 2017 Win64"

pause