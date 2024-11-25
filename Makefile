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
CFLAGS := -Wall -Wextra -fopenmp -DN=$(N)
LDFLAGS := -fopenmp -lm

# Headers
HEADERS := $(wildcard $(INCLUDE_DIR)/*.h)

# Default target
all: $(TARGET)

# Create directories
$(OBJ_DIR):
	mkdir -p $(OBJ_DIR)

$(BIN_DIR):
	mkdir -p $(BIN_DIR)

# Compile the imp_par file with appropriate flags
$(OBJ_DIR)/imp_par.o : $(SRC_DIR)/imp_par.c $(INCLUDE_DIR)/imp_par.h | $(OBJ_DIR)
	$(CC) -DN=$(N) -O2 -ftree-vectorize -funroll-loops -fopt-info -I$(INCLUDE_DIR) -c $< -o $@

# Compile the imp_seq file with appropriate flags
$(OBJ_DIR)/seq.o : $(SRC_DIR)/seq.c $(INCLUDE_DIR)/seq.h | $(OBJ_DIR)
	$(CC) -DN=$(N) -I$(INCLUDE_DIR) -c $< -o $@

# Compile the rest of the files
$(OBJ_DIR)/%.o: $(SRC_DIR)/%.c $(HEADERS) | $(OBJ_DIR)
	$(CC) $(CFLAGS) -I$(INCLUDE_DIR) -c $< -o $@

# Link
$(TARGET): $(OBJ) | $(BIN_DIR)
	$(CC) $(OBJ) $(LDFLAGS) -o $@

# Clean
clean:
	rm -rf $(OBJ_DIR) $(BIN_DIR)

.PHONY: all clean