# compiler_defs.mk - Compiler definition and flags

include proj_defs.mk

# Compiler
CC = clang
CXX = clang++
CCVERSION = $(shell clang --version | grep ^clang | cut -f1 -d"." | sed 's/^.* //g' )

# Standard flags
STD_FLAGS = -MD -MP -m64 -Wall -Wextra -fPIC -fno-builtin-memset -fvisibility=hidden \
		    -mcmodel=small -mstack-alignment=16 -mstackrealign -std=c17 -mno-mmx -mno-sse

# Optimization flags
OPT_FLAGS = -O2

# SecV mandatory flags
SECV_FLAGS = -Wdouble-promotion -Wshadow -Wconversion -Wmissing-prototypes -Wpointer-arith \
			 -Wuninitialized -Wunreachable-code -Wunused-function -Werror -D_FORTIFY_SOURCE=2 \
			 -fno-zero-initialized-in-bss -fstack-protector-strong -fno-jump-tables

CET_FLAGS = -mshstk -fcf-protection

# Combined flags
CFLAGS = $(STD_FLAGS) $(PROJ_FLAGS) $(OPT_FLAGS) $(SECV_FLAGS) $(CET_FLAGS) $(PRODUCTION_FLAGS)

# Entry pointer for the linker
MODULE_ENTRY_POINT = pseamldr_entry_point

# Linker flags
LINKER_SCRIPT = $(PROJ_DIR)/tdx_linker_script.lds

LDFLAGS = -Wl,-shared -Wl,-pie -Wl,-e,$(MODULE_ENTRY_POINT) -Wl,-z,relro -Wl,-z,now -Wl,--wrap=__stack_chk_fail \
		  -disable-red-zone -nostartfiles -Wl,-T,$(LINKER_SCRIPT)

