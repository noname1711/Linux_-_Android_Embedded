.PHONY: clean setup all server client

CUR_DIR := .
INC_DIR := $(CUR_DIR)/inc
SRC_DIR := $(CUR_DIR)/src
OBJ_DIR := $(CUR_DIR)/obj
BIN_DIR := $(CUR_DIR)/bin

CC := gcc
CFLAGS := -I$(INC_DIR) -Wall -Wextra

PROTO ?= ipv4
TYPE  ?= stream
PORT  ?= 8080
HOST  ?= 127.0.0.1

SRC_PATH := $(SRC_DIR)/$(PROTO)/$(TYPE)
SERVER_SRC := $(SRC_PATH)/server.c
CLIENT_SRC := $(SRC_PATH)/client.c

SERVER_OBJ := $(OBJ_DIR)/$(PROTO)-$(TYPE)-server.o
CLIENT_OBJ := $(OBJ_DIR)/$(PROTO)-$(TYPE)-client.o

SERVER_BIN := $(BIN_DIR)/$(PROTO)-$(TYPE)-server
CLIENT_BIN := $(BIN_DIR)/$(PROTO)-$(TYPE)-client

setup:
	@mkdir -p $(OBJ_DIR) $(BIN_DIR)

clean:
	@rm -rf $(OBJ_DIR) $(BIN_DIR)

$(SERVER_OBJ): $(SERVER_SRC) | setup
	$(CC) $(CFLAGS) -c $< -o $@

$(CLIENT_OBJ): $(CLIENT_SRC) | setup
	$(CC) $(CFLAGS) -c $< -o $@

$(SERVER_BIN): $(SERVER_OBJ)
	$(CC) $< -o $@

$(CLIENT_BIN): $(CLIENT_OBJ)
	$(CC) $< -o $@


all: $(SERVER_BIN) $(CLIENT_BIN)


server: $(SERVER_BIN)
	@echo "Running server [$(PROTO)/$(TYPE)]"
ifeq ($(PROTO),unix)
	$(SERVER_BIN)
else
	$(SERVER_BIN) $(PORT)
endif

client: $(CLIENT_BIN)
	@echo "Running client [$(PROTO)/$(TYPE)]"
ifeq ($(PROTO),unix)
	$(CLIENT_BIN)
else
	$(CLIENT_BIN) $(PORT) $(HOST)
endif
