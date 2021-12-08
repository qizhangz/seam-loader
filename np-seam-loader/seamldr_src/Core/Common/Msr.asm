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
;  Procedure:	readMsr
; 
;  Input:	msr - MSR number to read
; 
;  Output:	pHiDword and pLoDword - pointers to high and low return values
; 
;  Description: Parts of MSR value are returned if pointers are not NULL
; 
;-----------------------------------------------------------------------------
;-----------------------------------------------------------------------------

__readMsr PROC NEAR USES eax edx ecx esi, msr:DWORD, pHiDword:PDWORD, pLoDword:PDWORD
        mov     ecx, msr
        rdmsr           
        mov     esi, pHiDword
        .IF esi != 0
          mov     [esi], edx
        .ENDIF
        mov     esi, pLoDword
        .IF esi != 0
          mov     [esi], eax
        .ENDIF
        ret
__readMsr ENDP


;-----------------------------------------------------------------------------
;-----------------------------------------------------------------------------
;  Procedure:	readMsr64
; 
;  Input:	msr - MSR number to read
; 
;  Output:	edx - hi value, eax - low value.
; 
;  Description: MSR value is returned as 64-bit value.
; 
;-----------------------------------------------------------------------------
;-----------------------------------------------------------------------------

__readMsr64 PROC NEAR USES ecx, msr:DWORD
        mov     ecx, msr
        rdmsr           
        ret
__readMsr64 ENDP


;-----------------------------------------------------------------------------
;-----------------------------------------------------------------------------
;  Procedure:	writeMsr
; 
;  Input:	msr - MSR number to read
; 
;  Output:	pHiDword and pLoDword - pointers to high and low values
; 
;  Description: MSR is written with passed-in values.
; 
;-----------------------------------------------------------------------------
;-----------------------------------------------------------------------------

__writeMsr PROC NEAR USES eax ecx edx, msr:DWORD, hiDword:DWORD, loDword:DWORD

        mov     ecx, msr                                        
        mov     edx, hiDword
        mov     eax, loDword
        wrmsr           
        ret
__writeMsr ENDP


;-----------------------------------------------------------------------------
;-----------------------------------------------------------------------------
;  Procedure:	readTsc64
; 
;  Input:	None
; 
;  Output:	edx - hi value, eax - low value.
; 
;  Description: TSC value is returned as 64-bit value.
; 
;-----------------------------------------------------------------------------
;-----------------------------------------------------------------------------

readTsc64 PROC NEAR
        rdtsc           
        ret
readTsc64 ENDP

;-----------------------------------------------------------------------------
;-----------------------------------------------------------------------------
;  Procedure:	calibrateTsc64
; 
;  Input:	None
; 
;  Output:	edx - hi value, eax - low value.
; 
;  Description: TSC Clk/Msec is returned as 64-bit value.
;               MSR_PLATFORM_INFO[15:8] * 133.33 MHz is frequency of TSC
; 
;-----------------------------------------------------------------------------
;-----------------------------------------------------------------------------

calibrateTsc64 PROC NEAR USES ecx 
        mov     ecx,    MSR_PLATFORM_INFO
        rdmsr
        and     eax, 0FF00h 
        shr     eax, 8  
        mov     ecx, 133330
        mul     ecx
        ret                                                        
calibrateTsc64 ENDP




_TEXT  ENDS

END
