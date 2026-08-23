
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
DEBUG_DIR   = $(OUT_DIR)/Debug

STARTUP     = startup_stm32f401xx.s
LINKER      = STM32F401XX_FLASH.ld

# ======================
# Flags
# ======================
CFLAGS  = -O2 -Wall -ffunction-sections -fdata-sections
CFLAGS += -mcpu=$(MCU) -mthumb -mfpu=$(FPU) -mfloat-abi=$(FLOAT_ABI)

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

SRC := $(call rwildcard,$(DEBUG_DIR))
SRC += system_init_stm32f4xx.c     #addition of system init from the scripts dir
OBJ := $(SRC:.c=.o)

# ======================
# Default Target
# ======================
all: $(OUT_DIR)/$(TARGET).hex
	@mv $(OUT_DIR)/*.elf $(BIN_DIR)
	@mv $(OUT_DIR)/*.hex $(BIN_DIR)
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
$(OUT_DIR)/$(TARGET).elf: build
	@echo "Linking ELF..."
	$(CC) $(OBJ) $(STARTUP) $(LDFLAGS) -o $@
	$(SIZE) $@

# ======================
# Generate HEX
# ======================
$(OUT_DIR)/$(TARGET).hex: $(OUT_DIR)/$(TARGET).elf
	@echo "Generating HEX..."
	$(OBJCOPY) -O ihex $< $@

# ======================
# Clean
# ======================
clean:
	@rm -rf $(OUT_DIR)
	@echo "Build directory cleaned"

# ======================
# Sync (same idea as yours)
# ======================
sync: copy_dirs
	@echo "Sources synced"

copy_dirs:
	@cp -r $(SRC_DIR) $(OUT_DIR)
	@mv $(OUT_DIR)/program $(DEBUG_DIR)