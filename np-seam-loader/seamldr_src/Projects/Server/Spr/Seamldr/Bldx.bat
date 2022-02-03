@echo off
REM ;**********************************************************************;
REM ;*                                                                    *;
REM ;* Intel Proprietary                                                  *;
REM ;*                                                                    *;
REM ;* Copyright 2021 Intel Corporation All Rights Reserved.              *;
REM ;*                                                                    *;
REM ;* Your use of this software is governed by the TDX Source Code       *;
REM ;* LIMITED USE LICENSE.                                               *;
REM ;*                                                                    *;
REM ;* The Materials are provided "as is," without any express or         *;
REM ;* implied warranty of any kind including warranties of               *;
REM ;* merchantability, non-infringement, title, or fitness for a         *;
REM ;* particular purpose.                                                *;
REM ;*                                                                    *;
REM ;**********************************************************************;

set VC2008=0

if /I [%1]==[REL_NT] (
  set BLD=REL_NT
) else (
  goto USAGE
)

if /I [%2]==[O1] (
  set SFX=O1
) else (
if /I [%2]==[CLEAN] (
  set SFX=
  goto SET_ROOT
) else (
if /I [%2]==[CLEANALL] (
  set SFX=
  goto SET_ROOT
) else (
  goto USAGE
)
)
)


if [%3] NEQ [] set ACM_USER_REV=%3


:SET_ROOT
set X=..

:ITERATE
if exist %X%\rootx.bat (
    goto ROOT_DIR_DONE
rem  Up to 7 nested levels. Should be enough...
    if [%X%] EQU [..\..\..\..\..\..\..] goto FAIL
) ELSE (
    set X=%X%\..
    goto ITERATE
)

:FAIL
set X=
echo Cannot find rootx.bat. Exiting...
goto :EOF

:ROOT_DIR_DONE
echo Found %X%\rootx.bat file. Calling...

set PROJECT_DIR=%CD%

pushd %X%
call    rootx.bat
popd

set X=

if /I [%2]==[CLEAN] (
  %VC_TOOLS_PATH%\nmake CLEAN
  goto :EOF
) else (
if /I [%2]==[CLEANALL] (
  %VC_TOOLS_PATH%\nmake CLEANALL
  goto :EOF
) else (
  FOR /F "usebackq" %%A IN ('pseamldr.so') DO (
	rem set PSEAMLDRSIZE=%%~zA 
	echo PSEAMLDR_SIZE EQU %%~zA > PSeamldrSize.equ 
  )
  rem echo PSEAMLDR_SIZE EQU %PSEAMLDRSIZE% > PSeamldrSize.equ 
  %VC_TOOLS_PATH%\nmake
rem  set BLD_DIR=BUILD\PRJ\%BLD%\%SFX%
rem  echo AVIV
rem  echo %BLD_DIR%
  rem py putPseamldr.py %BLD_DIR%\_SEAMLDR_%BLD%_%SFX%.map %BLD_DIR%\_SEAMLDR_%BLD%_%SFX%.exe pseamldr.so pseamldr.so.consts
)
)

echo Unset PRJ_STR environment variable
set PRJ_STR=
goto :EOF

:USAGE
echo.
echo BLD ^<Build_Type^> ^<Optimization^> [Build_Date]
echo BLD ^<Build_Type^> ^<Clean_Option^>
echo.
echo Where: Build_Type   = REL_NT
echo        Optimization = O1
echo        Build_Date   = YYYYMMDD
echo        Clean_Option = CLEAN ^| CLEANALL
echo.

