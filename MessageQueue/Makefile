.PHONY: clean all setup 

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

create_sender: setup
	$(CC) -c $(SRC_DIR)/sender.c -o $(OBJ_DIR)/sender.o

create_receiver: setup
	$(CC) -c $(SRC_DIR)/receiver.c -o $(OBJ_DIR)/receiver.o

all_sender: create_sender
	$(CC) $(OBJ_DIR)/sender.o -o $(BIN_DIR)/sender
	$(BIN_DIR)/sender

all_receiver: create_receiver
	$(CC) $(OBJ_DIR)/receiver.o -o $(BIN_DIR)/receiver
	$(BIN_DIR)/receiver

all: all_sender all_receiver
