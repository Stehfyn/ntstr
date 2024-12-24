@echo off
setlocal enabledelayedexpansion
cd "%~dp0"

for /f "usebackq tokens=*" %%a in (`call "%ProgramFiles(x86)%\Microsoft Visual Studio\Installer\vswhere.exe" -latest -prerelease -products * -requires Microsoft.Component.MSBuild -property installationPath`) do (
   set "VSINSTALLPATH=%%a"
)

if not defined VSINSTALLPATH (
   echo No Visual Studio installation detected.
   exit /b 0
)

set out=out
set sources=..\..\ntstr.c
set cflags=/nologo /O2 /Oi /MT /std:c11 /Wall /WX /wd4710 /D _NDEBUG /D UNICODE /D _UNICODE
set libs=ntdll.lib vcruntime.lib

set "build_target=/?"

if exist "%VSINSTALLPATH%\VC\Auxiliary\Build\vcvarsall.bat" (
	rmdir /s /q %out%
	mkdir %out%
	pushd %out%
	mkdir bin

	call :%%build_target%% ntstr x86 3>&1 >nul
	call :%%build_target%% ntstr x64 3>&1 >nul
	popd
)

if "%0" == ":%build_target%" (
	echo Build Target %1-%2 @call:
	call "%VSINSTALLPATH%\VC\Auxiliary\Build\vcvarsall.bat" %2
	
	rmdir /s /q %2 2>nul
	mkdir %2
	pushd %2

	echo. && echo %2 hook 
	cl %cflags% /Tc %sources% %libs% /link /MACHINE:%2 /OUT:%1%2.exe
	copy %1%2.exe ..\bin\%1%2.exe

	popd
)>&3

exit /b 0