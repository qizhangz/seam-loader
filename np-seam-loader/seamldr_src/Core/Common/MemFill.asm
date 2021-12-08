;**********************************************************************;
;*                                                                    *;
;* Intel Proprietary                                                  *;
;*                                                                    *;
;* Copyright 2021 Intel Corporation All Rights Reserved.              *;
;*                                                                    *;
;* Your use of this software is governed by the TDX Source Code       *;
;* LIMITED USE LICENSE.                                               *;
;*                                                                    *;
;* The Materials are provided "as is," without any express or         *;
;* implied warranty of any kind including warranties of               *;
;* merchantability, non-infringement, title, or fitness for a         *;
;* particular purpose.                                                *;
;*                                                                    *;
;**********************************************************************;

.586p

	OPTION LANGUAGE:C
        
	.XLIST
        include common.inc
	.LIST


_TEXT  SEGMENT 'CODE' USE32  BYTE
        
;-----------------------------------------------------------------------------
;-----------------------------------------------------------------------------
;  Procedure:	MemFill
; 
;  Input:	Start - start address
;               Len - length to fill in bytes.
;               Val - pattern to store
; 
;  Output:	None
; 
;  Description:	Initializes "Len" bytes of memory with pattern "Val"
;               beginning from "Start" address. "D" bit is assumed clear. 
; 
;-----------------------------------------------------------------------------
;-----------------------------------------------------------------------------
MemFill PROC NEAR USES edi ecx, Start:PBYTE, Len:DWORD, Val:DWORD
        mov     edi, Start
        mov     eax, Val
        mov     ecx, Len
        shr     ecx, 2
        rep     stosd
        mov     ecx, Len
        and     ecx, 3
        rep     stosb
        ret        
MemFill ENDP

_TEXT  ENDS

END