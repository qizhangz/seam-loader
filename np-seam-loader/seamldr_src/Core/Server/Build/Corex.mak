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
                       
!message ===== In make file $(ACM_ROOT_DIR)\Core\Server\Build\corex.mak =====
# 
# Assign LIB_DIR to target core lib build
#

!if "$(LIB_DIR)" == ""

LIB_DIR      = $(ACM_ROOT_DIR)\Core\Server\Lib\Core\$(BLD)\$(SFX)

!endif

# 
# Do not assign SERVER LIB_DIR and OBJ_DIR. Will use either overrides provided
# by project or default location if built as standalone
#
                          
!include $(ACM_ROOT_DIR)\Core\Server\Build\ACMX.MAK
 
#
# NMAKE evaluates targets in the order in which they appear in make file, not in
# order in which they are listed in dependency line of ALL target.
#

all: 
#
# Call nmake to build core lib first
#           
        @echo ===== Performing CORE LIB BUILD step =====
        cd $(CORE_BUILDDIR)
        @echo MKF_FLAGS=$(MKF_FLAGS) 2
        @$(NMAKE) /F $(CORE_BUILDDIR)\corex.mak LIB_DIR=$(LIB_DIR) "MKF_FLAGS=$(MKF_FLAGS)"
        cd $(SERVER_CORE_BUILDDIR)
#
# Call nmake to build prebuild info
#
        @$(NMAKE) /F $(SERVER_CORE_BUILDDIR)\corex.mak prebuild SERVER_LIB_DIR=$(SERVER_LIB_DIR) MKF_FLAGS=$(MKF_FLAGS)
        @$(NMAKE) /F $(SERVER_CORE_BUILDDIR)\corex.mak TASK=BUILD build SERVER_LIB_DIR=$(SERVER_LIB_DIR) MKF_FLAGS=$(MKF_FLAGS)

                  
prebuild:
        @echo ===== Performing PREBUILD step =====
        @echo > nul <<SERVER_listconvert.txt<<SERVER_listexclude.txt        
common64.h
<<
build
lib
<<
!if !DEFINED(SKIP_PREBUILD)
        @echo $(PREBUILD) /R $(SERVER_CORE_DIR) /T $(SERVER_LIB_DIR)\SERVER_prebuild.incl /C SERVER_listconvert.txt /E SERVER_listexclude.txt /M CORE
        @$(PREBUILD) /R $(SERVER_CORE_DIR) /T $(SERVER_LIB_DIR)\SERVER_prebuild.incl /C SERVER_listconvert.txt /E SERVER_listexclude.txt /M CORE
!endif

build:
        @echo ===== Performing BUILD step ===== 
        @$(NMAKE) /F $(SERVER_CORE_BUILDDIR)\corex.mak TASK=ALL_BUILD all_build SERVER_LIB_DIR=$(SERVER_LIB_DIR) MKF_FLAGS=$(MKF_FLAGS)

all_build: $(SERVER_LIB_DIR)\$(SERVER_LIB) $(SERVER_LIB_DIR)\$(SERVER64_LIB)
                 
!if "$(TASK)"=="ALL_BUILD"

!message ===== Including $(SERVER_LIB_DIR)\prebuild.incl =====
!include  $(SERVER_LIB_DIR)\SERVER_prebuild.incl

!endif                                                                    
     
     
!message ===== inside Corex.mak looking for tmp file =====                                                 
$(SERVER_LIB_DIR)\$(SERVER_LIB): $(HEADERS) $(OBJECTS32)
        $(LIB) $(LIB_FLAGS) /OUT:$(SERVER_LIB_DIR)\$(SERVER_LIB) $(OBJECTS32)

$(SERVER_LIB_DIR)\$(SERVER64_LIB): $(HEADERS) $(OBJECTS64)
        $(LIB64) $(LIB64_FLAGS) /OUT:$(SERVER_LIB_DIR)\$(SERVER64_LIB) $(OBJECTS64)

clean:
    - rd /S /Q $(SERVER_LIB_DIR)
