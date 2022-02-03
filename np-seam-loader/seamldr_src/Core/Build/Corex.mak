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

!message ===== In make file $(ACM_ROOT_DIR)\Core\Build\corex.mak =====

# 
# Do not assign LIB_DIR and OBJ_DIR. Will use either overrides provided
# by project or default location if built as standalone
#

!include $(ACM_ROOT_DIR)\Core\Build\ACMX.MAK

#
# NMAKER evaluates targets in the order in which they appear in make file, not in
# order in which they are listed in dependency line of ALL target.
#

all: 
#
# Call nmake to build prebuild info
#
        @echo MKF_FLAGS=$(MKF_FLAGS) 3
        $(NMAKE) /F $(CORE_BUILDDIR)\corex.mak  LIB_DIR=$(LIB_DIR) prebuild
        $(NMAKE) /F $(CORE_BUILDDIR)\corex.mak TASK=BUILD LIB_DIR=$(LIB_DIR) OBJ_DIR=$(OBJ_DIR) build

                  
prebuild:
        @echo ===== Performing PREBUILD step =====
        @echo > nul <<listconvert.txt<<listexclude.txt
common.h
common64.h
header.h
acmerr.h
sinitcom.h
sinitmle.h
sinitheap.h
<<
client
build
server
lib
<< 
!if !DEFINED(SKIP_PREBUILD)
        $(PREBUILD) /R $(CORE_DIR) /T $(LIB_DIR)\prebuild.incl /C listconvert.txt /E listexclude.txt /M CORE
!endif

build:
        @echo ===== Performing BUILD step =====
        $(NMAKE) /F $(CORE_BUILDDIR)\corex.mak TASK=ALL_BUILD LIB_DIR=$(LIB_DIR) all_build 

all_build: $(LIB_DIR)\$(CORE_LIB) $(LIB_DIR)\$(CORE64_LIB)
                 
!if "$(TASK)"=="ALL_BUILD"

!message ===== Including $(LIB_DIR)\prebuild.incl =====
!include  $(LIB_DIR)\prebuild.incl

!endif
                                                      
$(LIB_DIR)\$(CORE_LIB): $(HEADERS) $(OBJECTS32)
        $(LIB) $(LIB_FLAGS) /OUT:$(LIB_DIR)\$(CORE_LIB) @<<
$(OBJECTS32)
<<

$(LIB_DIR)\$(CORE64_LIB): $(HEADERS) $(OBJECTS64)
        $(LIB64) $(LIB64_FLAGS) /OUT:$(LIB_DIR)\$(CORE64_LIB) @<<
$(OBJECTS64)
<<

clean:
    - rd /S /Q $(LIB_DIR)
