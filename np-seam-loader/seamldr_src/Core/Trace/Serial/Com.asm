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
        INCLUDE common.inc
	.LIST

        ;
        ; Not safe but faster
        ; 
        ;SET_BRATE       = 0
        
EDATA32	SEGMENT	PUBLIC USE32
        ASSUME ds:EDATA32

        ComAddr     dw	0               ; COM1=3F8h, COM2=2F8h, COM3=3E8h, COM4=2E8h 
        LineStat    dw	0               ; COM1=3F8h, COM2=2F8h, COM3=3E8h, COM4=2E8h 
        XmitErr     dw  0
        Once        dw  0
EDATA32 ENDS

_TEXT  SEGMENT 'CODE' USE32  BYTE

        InitSio PROTO   
        Wait1ms proto
                  
;-----------------------------------------------------------------------------
;-----------------------------------------------------------------------------
;  Procedure:   XmitChar
; 
;  Input:	bl - char to send
; 
;  Output:	None
; 
;  Description:	Send char to UART
; 
;-----------------------------------------------------------------------------
;-----------------------------------------------------------------------------

XmitChar PROC NEAR

        .REPEAT
          mov     dx, [LineStat]        ; Point to line status register
          in      al, dx                ; Get status byte            
        .UNTIL  al & 20h                ; Test if ready to transmit character
        xchg    al, bl

        mov     dx, [ComAddr]           ; Point to port
        out     dx, al                     
        clc
        ret  
XmitChar ENDP

;-----------------------------------------------------------------------------
;-----------------------------------------------------------------------------
;  Procedure:	XmitReadStatus
; 
;  Input:	None
; 
;  Output:	Z if status is met
; 
;  Description:	Check for transmit buffer empty
; 
;-----------------------------------------------------------------------------
;-----------------------------------------------------------------------------
XmitReadStatus PROC NEAR USES ax dx
        
        mov     dx, [LineStat]           ; Point to line status register

        in      al, dx                  ; Get status byte            
        ;
        ; This inverts bit5 and makes its Z value an indication that
        ; xmit is ready
        ;
        not     al                       
        test    al, 20h                 ; Test if ready to transmit character

        ret
XmitReadStatus ENDP

;-----------------------------------------------------------------------------
;-----------------------------------------------------------------------------
;  Procedure:	SerialOut
; 
;  Input:	pStr - string to send.
; 
;  Output:	None
; 
;  Description:	Send a string to UART.
; 
;-----------------------------------------------------------------------------
;-----------------------------------------------------------------------------
ComSerialOut PROC NEAR USES esi eax ebx edx, pStr:PBYTE 
        .IF XmitErr != 0
          ret
        .ENDIF
        
        .IF ![Once]
          call    InitPort
          ;
          ; Send single char
          ;
          mov     dx, [ComAddr]           ; Point to port
          mov     al, 0
          out     dx, al                     
          ;
          ; Wait for line status to be ready.
          ;
          mov     ecx, 2
          .REPEAT
            push    ecx
            call    Wait1ms
            pop     ecx  

            call    XmitReadStatus
            .BREAK .IF ZERO?      
          .UNTILCXZ 
                                   
          ;
          ; Report error if not ready - this silently disables serial output
          ;
          .IF ecx == 0
            mov   XmitErr, 1
            ret
          .ENDIF
          
          inc   Once
        .ENDIF
        
        mov     esi, pStr

        .REPEAT
          .IF     BYTE PTR [esi]==0
            ret   
          .ELSEIF BYTE PTR [esi]==0Ah 
            mov     bl, 0Dh
            call    XmitChar 
          .ENDIF          
          mov     bl, [esi]
          call    XmitChar
          inc     esi
        .UNTIL 0 
        
        ret
ComSerialOut ENDP

;-----------------------------------------------------------------------------
;-----------------------------------------------------------------------------
;  Procedure:	
; 
;  Input:	None
; 
;  Output:	None
; 
;  Description:	
; 
;-----------------------------------------------------------------------------
;-----------------------------------------------------------------------------
;retNotZero PROC NEAR USES eax
;        or      eax, 1
;        ret
;retNotZero ENDP

;-----------------------------------------------------------------------------
;-----------------------------------------------------------------------------
;  Procedure:	InitPort
; 
;  Input:	None
; 
;  Output:	None
; 
;  Description:	Init UART.
; 
;-----------------------------------------------------------------------------
;-----------------------------------------------------------------------------
        ;
        ; Set the baud rate divisor at chip level to 115 KBd
        ;
        BDRMSB      EQU	0             
        BDRLSB      EQU	1 

InitPort PROC USES eax
        ;
        ; Get COM addr from BDA
        ; 

	  mov	ComAddr, 3F8h 

	mov	ax, ComAddr 
	add	ax, 5 
	mov	LineStat, ax
        
IFDEF INIT_SIO
        INVOKE  InitSio
        .IF eax != 0
          ;
          ; Set baud rate
          ; 
          mov	dx, [ComAddr] 
          add	dx, 3                   ; Point to line control register
          mov	al, 87h                 ; DLAB bit.
          out	dx, al           
          
          sub   dx, 2                   ; Point to MSB of baud rate divisor
          mov	al, BDRMSB 
          out	dx, al 
          
          dec	dx                      ; Point to LSB of baud rate divisor
          mov	al, BDRLSB 
          out	dx, al 
          
          add	dx, 3                   ; Point to line control register
          mov   al, 3                   ; 8 data bits, 1 stop bit, no parity.
          out	dx, al
                               
          
          call    Wait1ms

        .ENDIF
          
ENDIF        
	ret
        
InitPort ENDP 


_TEXT  ENDS
END
