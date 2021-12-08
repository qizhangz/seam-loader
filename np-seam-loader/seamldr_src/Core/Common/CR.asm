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
        include acmcom.equ
	.LIST

_TEXT  SEGMENT 'CODE' USE32  BYTE

;-----------------------------------------------------------------------------
;-----------------------------------------------------------------------------
;  Procedure:   Update_CRx
; 
;  Input:	None
; 
;  Output:	None
; 
;  Description:	set CR0 and CR4. This procedure destroys registers
;                                   
;-----------------------------------------------------------------------------
;-----------------------------------------------------------------------------
Update_CRx PROC NEAR
        ;
        ; SEAMLDR should follow the spec and initialize it explicitly to the value 0x21 (PE + NE bit set, all others are cleared)
        ;                   
        mov	eax, CR0_PE + CR0_NE
        mov	cr0, eax
        
        mov eax, cr4
        or eax, CR4_PAE or CR4_SMXE
        mov cr4, eax

        ret
Update_CRx ENDP

_TEXT  ENDS

END
