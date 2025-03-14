@echo off
setlocal

set COMPILER=gcc
set OUTPUT=example.exe

set SOURCES=example.c src/LinPrint.c src/WinPrint.c
set CFLAGS=-Wall -Wextra -std=c23

where %COMPILER% >nul 2>&1
if %errorlevel% neq 0 (
    echo Error: GCC compiler not found. Please install MinGW or another compatible compiler.
    exit /b 1
)

echo Compiling...
%COMPILER% %CFLAGS% %SOURCES% -o %OUTPUT%

if %errorlevel% neq 0 (
    echo Compilation failed.
    exit /b 1
)

echo Compilation successful! Run .\%OUTPUT% to execute.

endlocal