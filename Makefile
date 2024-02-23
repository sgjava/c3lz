# Change this as needed
export ZCCCFG := $(HOME)/z88dk/lib/config

# Compiler
CC = zcc

# Compiler flags
CFLAGS = +cpm -mz80 -vn -clib=new -I$(HOME)/z88dk/include -I./include

# Source directory
SRC_DIR = src
# Object directory
BUILD_DIR = build

# Find all subdirectories of src
SUBDIRS := $(wildcard $(SRC_DIR)/*)
# Generate object file names
OBJS := $(patsubst $(SRC_DIR)/%, $(BUILD_DIR)/%, $(patsubst %.c, %.o, $(foreach dir,$(SUBDIRS),$(wildcard $(dir)/*.c))))

# Targets
all: $(OBJS)

# Rule to compile C source files into object files
$(BUILD_DIR)/%.o: $(SRC_DIR)/%.c
	@mkdir -p $(dir $@)
	$(CC) $(CFLAGS) -c $< -o $@

# Clean rule
clean:
	@rm -rf $(BUILD_DIR)

.PHONY: all clean
