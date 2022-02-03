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

!message ===== In make file $(ACM_ROOT_DIR)\Core\Server\Build\acmx.mak =====

!include $(ACM_ROOT_DIR)\Core\Build\ACMX.MAK 

#
# Directories.
# Four directories come from batch files:
# ACM_ROOT_DIR, TOOLS_PATH, and TOOL64_PATH come from ROOT.BAT located in the
# root of the ACM tree.                        
#
# PROJECT_DIR, BLD and SFX come from build batch file located in the project
# tip
#
# Others are defined here.
#                                   
SERVER_CORE_DIR			= $(CORE_DIR)\SERVER

SERVER_CORE_BUILDDIR		= $(SERVER_CORE_DIR)\Build

SERVER_INCLUDE_DIR	        = $(SERVER_CORE_DIR)\Include

LIB_BASE=$(ACM_ROOT_DIR)\Core\Server\Lib\$(BLD)
SERVERCORE_LIBDIR		= $(LIB_BASE)\Server

SERVERCORE_INC_DEPS		= \
          $(SERVERCORE_INCLUDE_DIR)\*.equ   \
				  $(SERVERCORE_INCLUDE_DIR)\*.h     \
				  $(SERVERCORE_LIBDIR)\asm-includes\*.inc     \

SERVERINC_DEPS			= $(SERVERCORE_INC_DEPS)

# Directories: LIB_DIR is folder where core libs are built;
#              OBJ_DIR is where project OBJ are built.
#
# Give ability to override where objects and library goes
# The server library will override this so server build of core is
# in unique location. This is needed because SERVER and server libs
# pass different make flags to core  
#

!if "$(SERVER_LIB_DIR)" == ""

SERVER_LIB_DIR=$(SERVER_CORE_DIR)\Lib\SERVER\$(BLD)\$(SFX)

!endif

!message SERVER_LIB_DIR = $(SERVER_LIB_DIR)

# 
# Tool flag updates.
#

!message ===== inside Acmx.mak looking for tmp file ===== 
LNK_FLAGS	=	$(LNK_FLAGS) /LIBPATH:$(SERVER_LIB_DIR)

LNK64_FLAGS	=	$(LNK64_FLAGS) /LIBPATH:$(SERVER_LIB_DIR)

H2INCX_FLAGS    =       $(BASE_H2INCX_FLAGS) -o $(SERVER_LIB_DIR)

# 
# Lib names. 
#          

SERVER_LIB = SERVER_$(BLD).LIB

SERVER64_LIB = SERVER64_$(BLD).LIB

# 
# Folders of all includes. Some may be empty. 
#                         

!if "$(PROJECT_DIR)"==""

SERVER_INCLUDES      = /I$(SERVER_INCLUDE_DIR) /I$(SERVER_LIB_DIR)

!else           

SERVER_INCLUDES      = /I$(PROJECT_DIR) /I$(SERVER_INCLUDE_DIR) /I$(SERVER_LIB_DIR)

!endif                             

ALL_INCLUDES      = $(SERVER_INCLUDES) $(CORE_INCLUDES)
                                                       
#               
# Dependencies. 
#               

SERVER_MAK_DEPS	= $(SERVER_CORE_BUILDDIR)\ACMX.MAK \
 		  $(SERVER_CORE_BUILDDIR)\COREX.MAK

SERVER_INC_DEPS	= $(SERVER_INCLUDE_DIR)\*.equ \
		  $(SERVER_INCLUDE_DIR)\*.h

ALL_DEPS        = $(SERVER_INC_DEPS) $(SERVER_MAK_DEPS) $(CORE_INC_DEPS) $(CORE_MAK_DEPS)


