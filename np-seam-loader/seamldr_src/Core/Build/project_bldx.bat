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

setlocal

set ACMTYPE=%1
set BLDTYPE=%2
set TARGET=%3
set REV=%4

if /I [%BLDTYPE%]==[ENG_NT] (
  set BLD=ENG_NT
) else (
if /I [%BLDTYPE%]==[ENG_TR] (
  set BLD=ENG_TR
  ) else (
if /I [%BLDTYPE%]==[REL_NT] (
  set BLD=REL_NT
) else (
  goto USAGE
)
)
)

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
echo Found %X%rootx.bat file. Calling...

pushd %X%
call    rootx.bat
popd

set X=

if [%REV%] NEQ [] set ACM_USER_REV=%REV%

set PROJECT_DIR=%CD%
set BLD_DIR_ROOT=%PROJECT_DIR%\BUILD\%BLD%
set LIB_BASE=%BLD_DIR_ROOT%\LIBS

if /I [%TARGET%]==[CLEAN] (
  %VC_TOOLS_PATH%\nmake clean
  goto :EOF
) else (
if /I [%TARGET%]==[CLEANALL] (
  %VC_TOOLS_PATH%\nmake cleanall
  goto :EOF
) else (

  IF NOT EXIST %BLD_DIR_ROOT%\makeflag.%TARGET% (
    echo Copying %BLD_DIR_ROOT%\makeflag.%TARGET%...
    copy makeflag.mak %BLD_DIR_ROOT%\makeflag.%TARGET%
  )

  fc /L /T makeflag.mak %BLD_DIR_ROOT%\makeflag.%TARGET% > nul 2>&1

  if errorlevel 1 (
    echo Running CLEANALL
    copy makeflag.mak %BLD_DIR_ROOT%\makeflag.%TARGET%
    %VC_TOOLS_PATH%\nmake cleanall
  )

  set SFX=%TARGET%
  %VC_TOOLS_PATH%\nmake
)
)
goto :EOF

:USAGE
echo.
echo BLD ENG_NT^|ENG_TR^|REL_NT [SUFFIX]^|[CLEAN[ALL]] [YYYYMMDD]

