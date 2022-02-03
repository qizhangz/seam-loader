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

!message ===== In make file $(ACM_ROOT_DIR)\Core\Build\acmx.mak =====

!IF "$(ACM_VERBOSE)" != "0"
VERBOSE		=
ENDVERBOSE	=
!ELSE
VERBOSE		= @
ENDVERBOSE	= 1>nul
!ENDIF
                  
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
CORE_DIR             = $(ACM_ROOT_DIR)\Core 

CORE_BUILDDIR	     = $(CORE_DIR)\Build

INCLUDE_DIR          = $(ACM_ROOT_DIR)\Core\Include

SERVER_INCLUDE_DIR   = $(ACM_ROOT_DIR)\Core\Server\Include

# Directories: LIB_DIR is folder where core libs are built;
#              OBJ_DIR is where project OBJ are built.
#
# Give ability to override where objects and library goes
# The server library will override this so server build of core is
# in unique location. This is needed because client and server libs
# pass different make flags to core  
#

!if "$(LIB_DIR)" == ""
LIB_DIR      = $(CORE_DIR)\Lib\$(BLD)\$(SFX)
!endif
!message LIB_DIR = $(LIB_DIR)

#
# Tool names.
#

ML                   = $(VERBOSE)$(VC_TOOLS_PATH)\ml
CL                   = $(VERBOSE)$(VC_TOOLS_PATH)\cl
LINK                 = $(VERBOSE)$(VC_TOOLS_PATH)\link
LIB                  = $(VERBOSE)$(VC_TOOLS_PATH)\lib
NMAKE                = $(VERBOSE)$(VC_TOOLS_PATH)\NMAKE /NOLOGO
DUMPBIN              = "$(VERBOSE)$(VC_TOOLS_PATH)\dumpbin"
ACM_PE2BIN           = $(VERBOSE)$(ACM_TOOLS_PATH)\acm_pe2bin
ACM_PE2BINx          = $(VERBOSE)$(ACM_TOOLS_PATH)\acm_pe2binx
H2INCX               = $(VERBOSE)$(ACM_TOOLS_PATH)\h2incx
REHASH               = $(VERBOSE)$(ACM_TOOLS_PATH)\rehash
PREBUILD             = $(VERBOSE)$(ACM_TOOLS_PATH)\prebuild64

ML64                 = $(VERBOSE)$(VC_TOOLS64_PATH)\ml64
CL64                 = $(VERBOSE)$(VC_TOOLS64_PATH)\cl
LINK64               = $(VERBOSE)$(VC_TOOLS64_PATH)\link
LIB64                = $(VERBOSE)$(VC_TOOLS64_PATH)\lib
             
# 
# Tool flags.
#

ML_FLAGS             = /nologo /Gd /c /Cx /WX /Zi /coff /Sa /Fl$(@R).lst /D__ASM__=1 $(MKF_FLAGS) $(ALL_INCLUDES)  
                       
ML64_FLAGS           = /nologo /c /Cx /WX /Zi /D__ASM__=1 $(ALL_INCLUDES)
                                                    
!if !DEFINED(ALL_CL_FLAGS)
ALL_CL_FLAGS=/O1
!endif

#
# BASE_CL_FLAGS macro contains major part of CL options, which can be updated
# in other make files to compose full CL_FLAGS option set. Such design helps
# prevent unneeded nesting leading to multiple repetition of the same option.
# Analogous design is used in other macros too.
#

IPP_CL_FLAGS         =  /D_TXT_ACM_=1 /D_IPP_LE=1 /D_ARCH_IA32=1
ML_FLAGS             =  $(ML_FLAGS) $(IPP_CL_FLAGS)

BASE_CL_FLAGS        = /nologo /c /GS /WX /Zp1 /Zi $(ALL_CL_FLAGS) /Oy- /Fa$@R.cod /FAscu /Fd$@R.pdb $(TDX_IO_FLAG) $(IPP_CL_FLAGS) $(MKF_FLAGS) $(ALL_INCLUDES)

LIB_FLAGS            = /NOLOGO

LIB64_FLAGS          = $(LIB_FLAGS)       

ACM_PE2BINx_FLAGS    =
                                                   
BASE_H2INCX_FLAGS         = -y -b -d2 -v -c -q 

H2INCX_FLAGS         = $(BASE_H2INCX_FLAGS) -o $(LIB_DIR)

REHASH_FLAGS         = -none -sha1 -out:base64   

#
# Pertinent note: DEBUG, TRACE and RELEASE builds are orthogonal to
# DEBUG and PRODUCTION modules.
# former differ by build switches, optimization etc, whereas latter are
# related to which signing key was used.
#
# In general DEBUG build implies that optimisation is disabled - best option
# for source code debug. Unfortunately we cannot fit into 64K if this is
# true. This is why default optimisation is set to O1 and can be selectively
# overwritten by project local makefile. Various listings are also generated.
#
# TRACE build is identical to debug build but includes trace messages.
#
# RELEASE build is expected to have optimisation on, though it is not
# enforced by build. Developer must disable optimization overrides in local
# makefile.
#
#

!if "$(BLD)"=="ENG_NT"

ML_FLAGS=$(ML_FLAGS)  /DMKF_ENGINEERING=1 /DMKF_TRACE=0

BASE_CL_FLAGS=$(BASE_CL_FLAGS) /DMKF_ENGINEERING=1 /DMKF_TRACE=0

LINKER_DEBUG_FLAG=/DEBUG

!elseif "$(BLD)"=="ENG_TR"

ML_FLAGS=$(ML_FLAGS) /DMKF_ENGINEERING=1 /DMKF_TRACE=1

BASE_CL_FLAGS=$(BASE_CL_FLAGS) /DMKF_ENGINEERING=1 /DMKF_TRACE=1

LINKER_DEBUG_FLAG=/DEBUG

!elseif "$(BLD)"=="REL_NT"

ML_FLAGS=$(ML_FLAGS) /DMKF_ENGINEERING=0 /DMKF_TRACE=0 /DMKF_BENCHMARKING=0

BASE_CL_FLAGS=$(BASE_CL_FLAGS) /DMKF_ENGINEERING=0 /DMKF_TRACE=0 /DMKF_BENCHMARKING=0

LINKER_DEBUG_FLAG=

!elseif "$(BLD)"=="REGRESS"

ML_FLAGS=$(ML_FLAGS) /DMKF_ENGINEERING=1 /DMKF_TRACE=1 /DMKF_REGRESSION=1

BASE_CL_FLAGS=$(BASE_CL_FLAGS) /DMKF_ENGINEERING=1 /DMKF_TRACE=1 /DMKF_REGRESSION=1

LINKER_DEBUG_FLAG=

!else

!ERROR ERROR: Unsupported build type (expecting ENG_NT/ENG_TR/REL_NT/REGRESS).

!endif
                
CL_FLAGS = $(BASE_CL_FLAGS) 

CL64_FLAGS = $(BASE_CL_FLAGS)

LNK_FLAGS            = /DYNAMICBASE /NOLOGO \
                       /MERGE:ADATA32=EDATA32 \
                       /MERGE:SDATA32=EDATA32 \
                       /MERGE:CODE32=.text \
                       /MERGE:.rdata=.comment \
                       /MERGE:.data=.text \
                       /base:0 \
                       -LAST:EDATA32 -LAST:.text -LAST:.comment \
                       /OPT:REF /OUT:$(OBJ_DIR)\$(ACM_NAME).exe \
                       /PDB:$(OBJ_DIR)\$(ACM_NAME).pdb \
                       /NODEFAULTLIB /ALIGN:4096 /IGNORE:4108 /IGNORE:4254 \
                       /MAP $(LINKER_DEBUG_FLAG) /ENTRY:$(ACM_ENTRYPOINT) \
                       /SUBSYSTEM:native /LIBPATH:$(LIB_DIR) \

LNK64_FLAGS          = /DYNAMICBASE /NOLOGO \
                       /MERGE:.rdata=.uncomment \
                       /base:0 \
                       -LAST:.text -LAST:.pdata -LAST:.comment \
                       /OPT:REF /OUT:$(OBJ_DIR)\$(ACM64_NAME).exe \
                       /PDB:$(OBJ_DIR)\$(ACM64_NAME).pdb \
                       /NODEFAULTLIB /ALIGN:4096 /IGNORE:4108 /IGNORE:4254 \
                       /MAP $(LINKER_DEBUG_FLAG) /ENTRY:$(ACM64_ENTRYPOINT) \
                       /SUBSYSTEM:native /LIBPATH:$(LIB_DIR) \

# 
# Lib names. 
#          

CORE_LIB = CORE_$(BLD).LIB

CORE64_LIB = CORE64_$(BLD).LIB
                                                   
# 
# Folders of all includes.
#                         

IPP_INCLUDES     =  /I$(CORE_DIR)\Crypto64\ipp\include

CORE_INCLUDES    = /I$(INCLUDE_DIR) /I$(SERVER_INCLUDE_DIR) $(IPP_INCLUDES) /I$(LIB_DIR) /I$(OBJ_DIR)

!if "$(PROJECT_DIR)"!=""

CORE_INCLUDES    = /I$(PROJECT_DIR) $(CORE_INCLUDES)

!endif

ALL_INCLUDES=$(CORE_INCLUDES)

#               
# Dependencies. 
#               
                
CORE_MAK_DEPS	= $(CORE_BUILDDIR)\ACMX.MAK \
 		  $(CORE_BUILDDIR)\COREX.MAK 

IPP_INC_DEPS    = $(CORE_DIR)\Crypto64\ipp\include\*.h \

CORE_INC_DEPS        = $(INCLUDE_DIR)\*.equ   \
                  $(INCLUDE_DIR)\*.h     \
                  $(IPP_INC_DEPS)     \
                  $(INC_DEPS) 
                  
!if "$(PROJECT_DIR)"!=""  
CORE_INC_DEPS        = $(CORE_INC_DEPS) $(PROJECT_DIR)\*.h \
                  $(INC_DEPS)
CORE_MAK_DEPS        = $(CORE_MAK_DEPS) $(PROJECT_DIR)\*.mak \
                  $(PROJECT_DIR)\makefile      
!endif
                                     
ALL_DEPS        = $(CORE_INC_DEPS) $(CORE_MAK_DEPS)
                  
# 
# To build core library as standalone need to prevent makeflag.h inclusion 
# 

!if "$(PROJECT_DIR)" == ""

!undef MKF_USE_MAKEFLAG_H

!endif

#
# Suffixes
#

.SUFFIXES :

.SUFFIXES : .exe .obj .c .asm .equ .inc .h

