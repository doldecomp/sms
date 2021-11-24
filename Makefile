# If defined, wine is not used.
NOWINE ?= 0

ifneq ($(findstring MINGW,$(shell uname)),)
  WINDOWS := 1
endif
ifneq ($(findstring MSYS,$(shell uname)),)
  WINDOWS := 1
endif

#-------------------------------------------------------------------------------
# Gamecube SDK and Codewarrior Includes
#-------------------------------------------------------------------------------

# Import the SDK path variable and set the paths.
SDK_BASE_PATH := $(SDK_BASE_PATH)
SDK_LIB_PATH  := $(SDK_BASE_PATH)/HW2/lib

# The Dec 2001 SDK is required for this, which doesn't use /include.
SDK_INC_PATH  := $(SDK_BASE_PATH)

# Check if SDK is not defined, error if not defined.
ifeq ($(SDK_BASE_PATH),)
$(error You have not defined SDK_BASE_PATH. Please ensure the Gamecube SDK is installed and point SDK_BASE_PATH as an environment variable to its location.)
endif

# Import the Codewarrior GC 1.1 path variable and set the include path as well.
CW_BASE_PATH := $(CW_BASE_PATH)
CW_INC_PATH  := $(CW_BASE_PATH)/PowerPC_EABI_Support

# Check if CW is not defined, error if not defined.
ifeq ($(CW_BASE_PATH),)
$(error You have not defined CW_BASE_PATH. Please ensure Codewarrior for Gamecube is installed and point CW_BASE_PATH as an environment variable to its location.)
endif

#-------------------------------------------------------------------------------
# Files
#-------------------------------------------------------------------------------

TARGET := sms_jp_r0

BUILD_DIR := build/$(TARGET)

SRC_DIRS := src                      \
            src/NPC                  \
            src/MarioUtil            \
            src/JSystem              \
            src/System               \
            src/MSL_C.PPCEABI.bare.H \

ASM_DIRS := asm                      \
            asm/JSystem              \
			asm/Runtime.PPCEABI.H    \
			asm/MSL_C.PPCEABI.bare.H \
			asm/TRK_MINNOW_DOLPHIN   \
			asm/base                 \
			asm/db                   \
			asm/os                   \
			asm/mtx                  \
			asm/dvd                  \
			asm/vi                   \
			asm/pad                  \
			asm/ai                   \
			asm/ar                   \
			asm/dsp                  \
			asm/card                 \
			asm/gx                   \
			asm/OdemuExi2            \
			asm/amcstubs             \
			asm/odenotstub           \
			asm/gd                   \
			asm/si                   \
			asm/exi                  \
			asm/thp                  \
			asm/THPPlayer            \
			asm/MarioUtil            \
			asm/M3DUtil              \
			asm/System               \
			asm/Strategic            \
			asm/Player               \
			asm/NPC                  \
			asm/MSound               \
			asm/MoveBG               \
			asm/Map                  \
			asm/GC2D                 \
			asm/Enemy                \
			asm/Camera               \
			asm/Animal               \

# Inputs
S_FILES := $(wildcard asm/*.s)
C_FILES := $(wildcard src/*.c)
CPP_FILES := $(wildcard src/*.cpp)
LDSCRIPT := $(BUILD_DIR)/ldscript.lcf

# Outputs
DOL     := $(BUILD_DIR)/main.dol
ELF     := $(DOL:.dol=.elf)
MAP     := $(BUILD_DIR)/$(TARGET).map

include obj_files.mk

#-------------------------------------------------------------------------------
# Tools
#-------------------------------------------------------------------------------

MWCC_VERSION := GC/1.2.5

# Programs
ifeq ($(WINDOWS),1)
  WINE :=
else
  WINE := wine
endif

ifeq ($(NOWINE),1)
  WINE :=
endif

AS      := $(DEVKITPPC)/bin/powerpc-eabi-as
CPP     := $(DEVKITPPC)/bin/powerpc-eabi-cpp -P
CC      := $(WINE) tools/mwcc_compiler/$(MWCC_VERSION)/mwcceppc.exe
LD      := $(WINE) tools/mwcc_compiler/$(MWCC_VERSION)/mwldeppc.exe
ELF2DOL := tools/elf2dol
SHA1SUM := sha1sum
PYTHON  := python
POSTPROC := tools/postprocess.py

# Options
INCLUDES := -i . -I- -i include -i src -ir "$(SDK_INC_PATH)" -ir "$(CW_INC_PATH)"

ASFLAGS := -m750cl -I include
LDFLAGS := -map $(MAP) -fp hard -nodefaults
# LDFLAGS := -fp hard -nodefaults
CFLAGS  := -Cpp_exceptions off -proc gekko -fp hard -O4 -nodefaults -msgstyle gcc $(INCLUDES) -enum int -rostr

# for postprocess.py
PROCFLAGS := -fsymbol-fixup -fprologue-fixup=old_stack

# hardcoded flags
$(BUILD_DIR)/src/System/FlagManager.o: FILE_UNIQUE_CFLAGS = -opt all,nostrength -inline all,level=1,deferred
$(BUILD_DIR)/src/System/ParamInst.o:   FILE_UNIQUE_CFLAGS = -use_lmw_stmw=off

#-------------------------------------------------------------------------------
# Recipes
#-------------------------------------------------------------------------------

### Default target ###

default: all

all: $(DOL)

ALL_DIRS := build $(BUILD_DIR) $(addprefix $(BUILD_DIR)/,$(SRC_DIRS) $(ASM_DIRS))

# Make sure build directory exists before compiling anything
DUMMY != mkdir -p $(ALL_DIRS)

.PHONY: tools

$(LDSCRIPT): ldscript.lcf
	$(CPP) -MMD -MP -MT $@ -MF $@.d -I include/ -I . -DBUILD_DIR=$(BUILD_DIR) -o $@ $<

$(DOL): $(ELF) | tools
	$(ELF2DOL) $< $@
	$(SHA1SUM) -c $(TARGET).sha1

clean:
	rm -fdr build
	$(MAKE) -C tools clean

tools:
	$(MAKE) -C tools

$(ELF): $(O_FILES) $(LDSCRIPT)
	@echo $(O_FILES) > build/o_files
	$(LD) $(LDFLAGS) -o $@ -lcf $(LDSCRIPT) @build/o_files

$(BUILD_DIR)/%.o: %.s
	$(AS) $(ASFLAGS) -o $@ $<
# 	$(PYTHON) $(POSTPROC) $(PROCFLAGS) $@

$(BUILD_DIR)/%.o: %.cpp
	$(CC) $(CFLAGS) $(FILE_UNIQUE_CFLAGS) -c -o $@ $<

$(BUILD_DIR)/%.o: %.c
	$(CC) $(CFLAGS) $(FILE_UNIQUE_CFLAGS) -c -o $@ $<

print-% : ; $(info $* is a $(flavor $*) variable set to [$($*)]) @true