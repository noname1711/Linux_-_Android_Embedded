.PHONY: clean setup create_objhw1 allhw1 create_objhw3 allhw3 writehw3 readhw3 create_objhw4 allhw4

CUR_DIR := .
INC_DIR := $(CUR_DIR)/inc
SRC_DIR := $(CUR_DIR)/src
OBJ_DIR := $(CUR_DIR)/obj
BIN_DIR := $(CUR_DIR)/bin
TEXT_DIR := $(CUR_DIR)/text

CC := gcc

setup:
	mkdir -p $(OBJ_DIR) $(BIN_DIR)

clean:
	rm -rf $(BIN_DIR) $(OBJ_DIR)

# Hw1
beforehw1:
	@cd $(TEXT_DIR) && cat test.txt

create_objhw1: setup
	$(CC) -c $(SRC_DIR)/hw1.c -o $(OBJ_DIR)/hw1.o

allhw1: create_objhw1
	$(CC) $(OBJ_DIR)/hw1.o -o $(BIN_DIR)/hw1
	$(BIN_DIR)/hw1
	@cd $(TEXT_DIR) && cat test.txt

# Hw3
NUM ?= 5
DATA ?= hello
FILE ?= data.txt

create_objhw3: setup
	$(CC) -c $(SRC_DIR)/hw3.c -o $(OBJ_DIR)/hw3.o

allhw3: create_objhw3
	$(CC) $(OBJ_DIR)/hw3.o -o $(BIN_DIR)/hw3

writehw3: allhw3
	$(BIN_DIR)/hw3 $(TEXT_DIR)/$(FILE) $(NUM) w $(DATA)

readhw3: allhw3
	$(BIN_DIR)/hw3 $(TEXT_DIR)/$(FILE) $(NUM) r chodu5arg

# Hw4
create_objhw4: setup
	$(CC) -c $(SRC_DIR)/hw4.c -o $(OBJ_DIR)/hw4.o

allhw4: create_objhw4
	$(CC) $(OBJ_DIR)/hw4.o -o $(BIN_DIR)/hw4
	./$(BIN_DIR)/hw4