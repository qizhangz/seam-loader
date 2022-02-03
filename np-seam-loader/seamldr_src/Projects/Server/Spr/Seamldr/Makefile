#;**********************************************************************;
#;*                                                                    *;
#;* Intel Proprietary                                                  *;
#;*                                                                    *;
#;* Copyright 2021 Intel Corporation All Rights Reserved.              *;
#;*                                                                    *;
#;* Your use of this software is governed by the TDX Source Code       *;
#;* LIMITED USE LICENSE.                                               *;
#;*                                                                    *;
#;* The Materials are provided "as is," without any express or         *;
#;* implied warranty of any kind including warranties of               *;
#;* merchantability, non-infringement, title, or fitness for a         *;
#;* particular purpose.                                                *;
#;*                                                                    *;
#;**********************************************************************;

!message ===== In make file $(PROJECT_DIR)\makefile =====

!include $(ACM_ROOT_DIR)\Core\Server\Build\ACMX.MAK 

#*****************************************************************************
#
# Name ACM
#
#*****************************************************************************

ACM             = SEAMLDR

ADDITIONAL_EXCLUDE_DIR=SINIT^

all1: build_extra all

#*****************************************************************************
#
# Build type selection for extra objects
#
#*****************************************************************************

!if "$(BLD)"=="ENG_NT"
!elseif "$(BLD)"=="ENG_TR"
!elseif "$(BLD)"=="REL_NT"
!else

!ERROR ERROR: Unsupported build type (expecting ENG_NT/ENG_TR/REL_NT).

!endif

build_extra:
!if DEFINED(EXTRA_OBJ_DIR)
        -md     $(EXTRA_OBJ_DIR)
!endif

!include $(ACM_ROOT_DIR)\Projects\Server\PROJECT.MAK

