.PHONY: clean setup

CUR_DIR := .
INC_DIR := $(CUR_DIR)/inc
SRC_DIR := $(CUR_DIR)/src
OBJ_DIR := $(CUR_DIR)/obj
BIN_DIR := $(CUR_DIR)/bin

CC := gcc

setup:
	mkdir -p $(OBJ_DIR) $(BIN_DIR)

clean:
	rm -rf $(BIN_DIR) $(OBJ_DIR)

# Hw1
create_objhw1: setup
	$(CC) -c $(SRC_DIR)/hw1.c -o $(OBJ_DIR)/hw1.o

allhw1: create_objhw1
	$(CC) $(OBJ_DIR)/hw1.o -o $(BIN_DIR)/hw1
	$(BIN_DIR)/hw1

# Hw2
create_objhw2: setup
	$(CC) -c $(SRC_DIR)/hw2.c -o $(OBJ_DIR)/hw2.o

allhw2: create_objhw2
	$(CC) $(OBJ_DIR)/hw2.o -o $(BIN_DIR)/hw2
	$(BIN_DIR)/hw2

# Hw3
create_objhw3: setup
	$(CC) -c $(SRC_DIR)/hw3.c -o $(OBJ_DIR)/hw3.o

allhw3: create_objhw3
	$(CC) $(OBJ_DIR)/hw3.o -o $(BIN_DIR)/hw3
	$(BIN_DIR)/hw3 "$(MES)"