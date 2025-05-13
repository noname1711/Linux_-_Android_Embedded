.PHONY: all clean setup run check-mem

# Directories
CUR_DIR := .
INC_DIR := $(CUR_DIR)/inc
SRC_DIR := $(CUR_DIR)/src
LIB_DIR := $(SRC_DIR)/lib
PEER_DIR := $(SRC_DIR)/peer
OBJ_DIR := $(CUR_DIR)/obj
BIN_DIR := $(CUR_DIR)/bin

# Compiler
CC := gcc
CFLAGS := -g -Wall -Wextra -I$(INC_DIR)

# File Lists
LIB_SRCS := $(wildcard $(LIB_DIR)/*.c)
LIB_OBJS := $(patsubst $(LIB_DIR)/%.c, $(OBJ_DIR)/%.o, $(LIB_SRCS))

PEER_SRC := $(PEER_DIR)/peer.c
PEER_OBJ := $(OBJ_DIR)/peer.o

TARGET := $(BIN_DIR)/peer

# Setup directories
setup:
	mkdir -p $(OBJ_DIR) $(BIN_DIR)

# Clean build
clean:
	rm -rf $(OBJ_DIR) $(BIN_DIR)

# Build object files for lib
create_lib_objs: setup
	$(CC) $(CFLAGS) -c $(LIB_DIR)/cli.c -o $(OBJ_DIR)/cli.o
	$(CC) $(CFLAGS) -c $(LIB_DIR)/connection.c -o $(OBJ_DIR)/connection.o
	$(CC) $(CFLAGS) -c $(LIB_DIR)/mySocket.c -o $(OBJ_DIR)/mySocket.o
	$(CC) $(CFLAGS) -c $(LIB_DIR)/mes.c -o $(OBJ_DIR)/mes.o
	$(CC) $(CFLAGS) -c $(LIB_DIR)/myLog.c -o $(OBJ_DIR)/myLog.o

# Build peer main object
create_peer_obj: setup
	$(CC) $(CFLAGS) -c $(PEER_SRC) -o $(PEER_OBJ)

# Link everything
all: create_lib_objs create_peer_obj
	$(CC) $(CFLAGS) -o $(TARGET) $(LIB_OBJS) $(PEER_OBJ)

# Run with custom port: make run PORT=8080
run: all
	@if [ -z "$(PORT)" ]; then \
		echo "Please provide a port, example: make run PORT=8080"; \
	else \
		echo "Running peer on port $(PORT)..."; \
		$(TARGET) $(PORT); \
	fi

# Check for memory leaks using Valgrind
check-mem: all
	@if [ -z "$(PORT)" ]; then \
		echo "Please provide a port, example: make check-mem PORT=8080"; \
	else \
		echo "Checking memory leaks for peer on port $(PORT) with Valgrind..."; \
		valgrind --leak-check=full --show-leak-kinds=all $(TARGET) $(PORT); \
	fi

	