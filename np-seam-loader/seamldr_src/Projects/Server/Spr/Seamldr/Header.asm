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
        include makeflag.inc               
        include header.inc
        include version_prt.equ
        include acmcom.equ                    
        include text_alignment_padding.inc
        include seamldr_const.equ
        include PSeamldrSize.equ
	.LIST

ADATA32 SEGMENT	PUBLIC USE32       
        
;        PUBLIC  ChipsetIdTblStart
;-----------------------------------------------------------------------;
; 				AC Module Header			;
;-----------------------------------------------------------------------;

HeaderStart ACM_HEADER_3 \
        <MCP_MODULE_TYPE,\
        MCP_MODULE_SUBTYPE,\
        HEADER_3_INIT_1,\
        MCP_MODULE_ID,\
        MCP_MODULE_FLAGS,\
        HEADER_INIT_2,\
        MCP_ACM_SVN,\
        MCP_SE_SVN, \
        HEADER_INIT_3,\
        GDT_SIZE,\
        OFFSET GdtBasePtr,\
        ACM_CODE_SELECTOR,\
        AcmEntryPoint,\
        HEADER_3_INIT_4>

AcmInfoTableStart ACM_INFO_TABLE \
        <INFO_TAB_INIT_1, \
        MCP_CHIPSET_ACM_TYPE, \
        ACM_INFO_TAB_VER, \
        INFO_TAB_INIT_2, \
        CHIPSET_ID_TBL_OFF, \
        OS_SINIT_TAB_VER, \
        0, \
        MCP_ACM_CAPABILITIES, \
        MCP_ACM_VER, \
        MCP_ACM_REVISION, \
        PROCESSOR_ID_TBL_OFF, \
        TPM_CAPABILITIES_TBL_OFF>
                            
include server_header.equ

        PSeamldrConstAsm P_SEAMLDR_CONSTS_t <>
        PSeamldrSizeAsm  dd PSEAMLDR_SIZE
        PSeamldrAsm      db PSEAMLDR_SIZE dup (0) 
        

ADATA32 ENDS

;------------------------------------------------------------------------;
;		     Stack Definition
;------------------------------------------------------------------------;
SDATA32 SEGMENT	PUBLIC USE32       

stackStart  db      (STACK_SIZE + text_alignment_padding) dup (0)       ; Create stack space



SDATA32 ENDS                         
                                   


END
