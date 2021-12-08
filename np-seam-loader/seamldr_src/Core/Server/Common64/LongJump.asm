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
	;include	common64.inc
	include	entry64.equ
    include acmcom64.equ  
    include common64.inc
	.LIST

	.DATA
	ALIGN 16

	.CODE

        ALIGN 16
        
;-----------------------------------------------------------------------------
;-----------------------------------------------------------------------------
;  Procedure:	LongJump
; 
;  Input:	Target - target address
; 
;  Output:	None
; 
;  Description: Executes long jump to target restoring stack
; 
;-----------------------------------------------------------------------------
;-----------------------------------------------------------------------------

LongJump PROC 
        push rcx
        ret NEAR
LongJump ENDP

END
