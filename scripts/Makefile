# ======================
# Project Configuration
# ======================
TARGET      = program
MCU         = cortex-m4
FPU         = fpv4-sp-d16
FLOAT_ABI   = hard

# ======================
# Toolchain
# ======================
CC          = arm-none-eabi-gcc
OBJCOPY     = arm-none-eabi-objcopy
SIZE        = arm-none-eabi-size

# ======================
# Directories
# ======================
SRC_DIR     = ../program
OUT_DIR     = ../build
BIN_DIR     = ../build/bin

STARTUP     = startup_stm32f401xx.s
LINKER      = STM32F401XX_FLASH.ld

# ======================
# Flags
# ======================
CFLAGS  = -O2 -Wall -ffunction-sections -fdata-sections
CFLAGS += -mcpu=$(MCU) -mthumb -mfpu=$(FPU) -mfloat-abi=$(FLOAT_ABI)
CFLAGS += -MMD -MP     # auto-track header dependencies so edits to .h files trigger rebuilds

LDFLAGS = -T$(LINKER)
LDFLAGS += -Wl,--gc-sections -Wl,-Map=$(OUT_DIR)/$(TARGET).map
LDFLAGS += -mcpu=$(MCU) -mthumb -mfpu=$(FPU) -mfloat-abi=$(FLOAT_ABI)




# ======================
# Ensure directories exist
# ======================
$(shell mkdir -p $(BIN_DIR))




# ======================
# Recursive wildcard
# ======================
define rwildcard
$(wildcard $(1)/*.c) $(foreach d,$(wildcard $(1)/*/),$(call rwildcard,$d))
endef

# Compile directly from the source tree - no copy/sync step required.
SRC := $(call rwildcard,$(SRC_DIR))
SRC += system_init_stm32f4xx.c     #addition of system init from the scripts dir
OBJ := $(SRC:.c=.o)
DEP := $(OBJ:.o=.d)

# ======================
# Default Target
# ======================
all: $(BIN_DIR)/$(TARGET).hex
	@echo "----------------------------------------------"
	@echo "---- STM32 HEX Generated Successfully  ----"
	@echo "------------- Made by eng-aly -------------"
	@echo "----------------------------------------------"

# ======================
# Compile
# ======================
%.o: %.c
	$(CC) $(CFLAGS) -c $< -o $@

# ======================
# Source Listing
# ======================
src: $(SRC)
	@echo "Compiling C files:"
	@$(foreach f,$(SRC),echo $(f);)

# ======================
# Build Objects
# ======================
build: src $(OBJ)
	@echo "Object files generated"

# ======================
# Link ELF
# ======================
$(BIN_DIR)/$(TARGET).elf: build
	@echo "Linking ELF..."
	$(CC) $(OBJ) $(STARTUP) $(LDFLAGS) -o $@
	$(SIZE) $@

# ======================
# Generate HEX
# ======================
$(BIN_DIR)/$(TARGET).hex: $(BIN_DIR)/$(TARGET).elf
	@echo "Generating HEX..."
	$(OBJCOPY) -O ihex $< $@

# ======================
# Clean
# ======================
clean:
	@rm -rf $(OUT_DIR)
	@rm -f $(OBJ) $(DEP)
	@echo "Build directory cleaned"

-include $(DEP)

.PHONY: all clean src build