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

	.XLIST
;	include	common64.inc
	include	entry64.equ
	.LIST

	.DATA
	ALIGN 16

	.CODE

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
MemFill PROC 
        push    rdi        
        push    rdx
        mov     rdi, rcx
        mov     rax, r8
        mov     rcx, rdx
        shr     rcx, 2
        rep     stosd
        pop     rcx
        and     rcx, 3
        rep     stosb
        pop     rdi
        ret        
MemFill ENDP

;-----------------------------------------------------------------------------
;-----------------------------------------------------------------------------
;  Procedure:	Movdir64B
; 
;  Input:	Source
;           Destination
; 
;  Output:	None
; 
;  Description:  Wrapper for a MOVDIR64B instruction
; 
;-----------------------------------------------------------------------------
;-----------------------------------------------------------------------------
Movdir64B PROC
    push       rdi
    push       rsi
    
    mov        rdi, rcx
    mov        rsi, rdx
;/*movdir64b op*/
    DB 066h
    DB 0Fh
    DB 038h
    DB 0F8h
;/*ModRM = RDI->RSI*/
    DB 037h
    
    pop        rsi
    pop        rdi
    
    ret
Movdir64B ENDP

END

       