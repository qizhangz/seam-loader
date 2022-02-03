# src_defs.mk - Sources, targets definitions and locations


# Makefile location - which is the project root dir
MAKEFILE_PATH := $(abspath $(lastword $(MAKEFILE_LIST)))
__PROJ_DIR := $(dir $(MAKEFILE_PATH))
# Remove the trailing slash '/' at the end of the directory string
PROJ_DIR := $(patsubst %/,%,$(__PROJ_DIR))

# Binary output location and name
RELEASE_DIR := $(PROJ_DIR)/bin/release
DEBUG_DIR := $(PROJ_DIR)/bin/debug
TARGET_NAME := pseamldr.so
RELEASE_TARGET := $(RELEASE_DIR)/$(TARGET_NAME)
DEBUG_TARGET := $(DEBUG_DIR)/$(TARGET_NAME)
RELEASE_TARGET_CONSTS := $(RELEASE_DIR)/$(TARGET_NAME).consts
DEBUG_TARGET_CONSTS := $(DEBUG_DIR)/$(TARGET_NAME).consts
OBJ_DIR_NAME := obj


# Source directories
SRC_DIRS := include src/common src/common/accessors src/common/crypto src/common/data_structures src/common/debug src/common/helpers src/common/memory_handlers src/common/x86_defs src/pseamldr_dispatcher src/pseamldr_dispatcher/api_calls
SRC_DIRS := $(foreach dir,$(SRC_DIRS),$(PROJ_DIR)/$(dir))


VPATH := $(SRC_DIRS)

# Source and headers files
C_SRC_FILES = $(foreach dir,$(SRC_DIRS),$(wildcard $(dir)/*.c))
ASM_SRC_FILES = $(foreach dir,$(SRC_DIRS),$(wildcard $(dir)/*.S))
SRC_FILES = $(C_SRC_FILES) $(ASM_SRC_FILES)
HEADER_FILES = $(foreach dir,$(SRC_DIRS),$(wildcard $(dir)/*.h))

# Objects
__C_OBJECTS = $(patsubst %.c, %.o, $(notdir $(C_SRC_FILES)))
__ASM_OBJECTS = $(patsubst %.S, %.o, $(notdir $(ASM_SRC_FILES)))

# Libraries
CRYPTO_LIB_BUILD_FLAVOR := RELEASE
ifndef CRYPTO_LIB_VERSION
CRYPTO_LIB_VERSION      := 2021.4
endif
CRYPTO_LIB_MAIN_DIR     := $(PROJ_DIR)/libs/ipp/ipp-crypto-ippcp_$(CRYPTO_LIB_VERSION)
CRYPTO_LIB_SRC_DIR      := $(CRYPTO_LIB_MAIN_DIR)/sources
CRYPTO_LIB_BUILD_PATH   := $(CRYPTO_LIB_MAIN_DIR)/_build/.build/$(CRYPTO_LIB_BUILD_FLAVOR)
CRYPTO_LIB_PATH         := $(CRYPTO_LIB_BUILD_PATH)/lib
CRYPTO_LIB_FILENAME     := libippcp_s_y8.a
CRYPTO_LIB              := -lippcp_s_y8

# Headers include path
INCLUDE_PATH := -I$(PROJ_DIR)/include -I$(CRYPTO_LIB_BUILD_PATH)/include -I$(PROJ_DIR)/src -I$(PROJ_DIR)/src/common

# Tools
TOOLS_DIR := $(PROJ_DIR)/tools
CONSTGEN_DIR := $(TOOLS_DIR)/constants_gen
CONSTGEN_C := $(CONSTGEN_DIR)/gen_constants.c
PAD_BINARY_PY := $(TOOLS_DIR)/pad_binary/pad_binary.py
