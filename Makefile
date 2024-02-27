# Change this as needed
export ZCCCFG := $(HOME)/z88dk/lib/config

# Compiler
CC = zcc

# Assembler
AS = z88dk-z80asm

# Compiler flags
CFLAGS = +cpm -mz80 -vn -I$(HOME)/z88dk/include -I./include

# Assembler flags
ASFLAGS = -d -x./build/lib/c3l

# Source directory
SRC_DIR = src
# Object directory
BUILD_DIR = build

# Find all subdirectories of src excluding src/demo and its subdirectories
SUBDIRS := $(shell find $(SRC_DIR) -mindepth 1 -maxdepth 1 -type d ! -name demo)

# Generate object file names
OBJS := $(patsubst $(SRC_DIR)/%, $(BUILD_DIR)/%, $(patsubst %.c, %.o, $(foreach dir,$(SUBDIRS),$(wildcard $(dir)/*.c))))

# Targets
all: $(OBJS)

# Rule to compile C source files into object files
$(BUILD_DIR)/%.o: $(SRC_DIR)/%.c
	@mkdir -p $(dir $@)
	$(CC) $(CFLAGS) -c $< -o $@

# Target to create lib files
lib: c3l.lst
	@mkdir -p $(BUILD_DIR)/lib
	$(AS) $(ASFLAGS) @$<

# Clean rule
clean:
	@rm -rf $(BUILD_DIR)

.PHONY: all lib clean
