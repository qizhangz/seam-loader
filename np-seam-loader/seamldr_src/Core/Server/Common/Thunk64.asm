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
;---------------------------------------------------------------------------
	OPTION LANGUAGE:C

	.XLIST
        include common.inc
        include common64.inc
        include header.inc        
        include acmcom.equ       
	.LIST

 EDATA32 SEGMENT PUBLIC USE32
         ASSUME  ds:EDATA32
         
         SeamldrCom64Data SEAMLDR_COM64_DATA <>
 EDATA32 ENDS
              

_TEXT  SEGMENT 'CODE' USE32 BYTE
                                                       
;-----------------------------------------------------------------------------
;-----------------------------------------------------------------------------
;  Procedure:	SeamldrThunk64
; 
;  Input:	None
; 
;  Output:	None
; 
;  Description:	Transfers control to 64-bit code
; 
;-----------------------------------------------------------------------------
;-----------------------------------------------------------------------------
SeamldrThunk64 PROC NEAR          
        ;
        ; System in compatibility mode
        ;
        mov     ecx, ACM_CODE64_SELECTOR ;ACM64_CODE
        push    ecx                     ; push ecx - in 32 bit mode
                                      
        mov     ecx, OFFSET LongMode
        push    ecx                     ; push ecx - in 32 bit mode
        retf                            ; will jump to LongMode label below
        ;
        ; Long mode.
        ;
LongMode:                     
        db      48h, 0B8h
        dq      0FFFFFFFFh              ; mov   rax, 00000000FFFFFFFFh
        db      48h, 21h, 0C4h          ; and   rsp, rax
        ;
        ; Call 64-bit entry point
        ;
        mov     esi, OFFSET AcmEntryPoint
        db      48h, 21h, 0C6h          ; and   rsi, rax
                            
        mov     ebx, cs:[esi - SIZEOF COM_DATA].COM_DATA.Code64Entry
        db      48h, 21h, 0C3h          ; and   rbx, rax
                            
        mov     ecx, OFFSET SeamldrCom64Data
        db      48h, 21h, 0C1h          ; and   rcx, rax
                            
        call    ebx                     ; call rbx   
        
        ;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
        ;;;;;;;;;;;;; TXT SHUTDOWN BELOW - WE NEVER RETURN HERE AFTER THE CALL ABOVE ;;;;;;;;;;;;;;
        ;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
        
        db      0Fh, 0Bh                ; ud2

SeamldrThunk64 ENDP


_TEXT  ENDS

END


