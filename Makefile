# Directories
SRC_DIR := src
OBJ_DIR := obj
BIN_DIR := bin
INCLUDE_DIR := include

# Target executable
TARGET := $(BIN_DIR)/main

# Source and object files
SRC := $(wildcard $(SRC_DIR)/*.c)
OBJ := $(SRC:$(SRC_DIR)/%.c=$(OBJ_DIR)/%.o)

# Compiler and flags
CC := gcc
CFLAGS := -Wall -Wextra -O3 -fopenmp
LDFLAGS := -fopenmp

# Headers
HEADERS := $(wildcard $(INCLUDE_DIR)/*.h)

# Default target
all: $(TARGET)

# Create directories
$(OBJ_DIR):
	mkdir -p $(OBJ_DIR)

$(BIN_DIR):
	mkdir -p $(BIN_DIR)

# Compile
$(OBJ_DIR)/%.o: $(SRC_DIR)/%.c $(HEADERS) | $(OBJ_DIR)
	$(CC) $(CFLAGS) -I$(INCLUDE_DIR) -c $< -o $@

# Link
$(TARGET): $(OBJ) | $(BIN_DIR)
	$(CC) $(OBJ) $(LDFLAGS) -o $@

# Clean
clean:
	rm -rf $(OBJ_DIR) $(BIN_DIR)

.PHONY: all clean