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
  
  
  FOR /F "usebackq" %%A IN ('pseamldr.so') DO set pseamldrsize=%%~zA
  echo PSEAMLDR_SIZE EQU %pseamldrsize% > PSeamldrSize.equ