.PHONY: all clean setup create_obj create_static create_shared link_static link_shared

CUR_DIR := .
INC_DIR := $(CUR_DIR)/inc
SRC_DIR := $(CUR_DIR)/src
OBJ_DIR := $(CUR_DIR)/obj
BIN_DIR := $(CUR_DIR)/bin
LIB_DIR := $(CUR_DIR)/lib
LIB_STATIC_DIR := $(LIB_DIR)/static
LIB_SHARED_DIR := $(LIB_DIR)/shared

CC := gcc
C_FLAGS := -c -fPIC -o
INCLUDES := -I$(INC_DIR)

OBJS := $(OBJ_DIR)/sqrt_lib.o
MAIN_OBJ := $(OBJ_DIR)/main.o

STATIC_LIB := $(LIB_STATIC_DIR)/libmysqrt.a
SHARED_LIB := $(LIB_SHARED_DIR)/libmysqrt.so

TARGET_STATIC := $(BIN_DIR)/use-static-library
TARGET_SHARED := $(BIN_DIR)/use-shared-library

setup:
	mkdir -p $(OBJ_DIR) $(BIN_DIR) $(LIB_DIR) $(LIB_STATIC_DIR) $(LIB_SHARED_DIR)

create_obj: setup
	$(CC) $(INCLUDES) $(C_FLAGS) $(MAIN_OBJ) $(SRC_DIR)/main.c
	$(CC) $(INCLUDES) $(C_FLAGS) $(OBJ_DIR)/sqrt_lib.o $(SRC_DIR)/sqrt_lib.c
	@ls -l $(OBJ_DIR)

create_static: create_obj
	ar rcs $(STATIC_LIB) $(OBJS)

create_shared: create_obj
	$(CC) -shared $(OBJS) -o $(SHARED_LIB)

link_static: create_static
	$(CC) $(MAIN_OBJ) $(STATIC_LIB) -o $(TARGET_STATIC) -lm
	
link_shared: create_shared
	$(CC) $(MAIN_OBJ) -L$(LIB_SHARED_DIR) -Wl,-rpath,$(LIB_SHARED_DIR) -lmysqrt -o $(TARGET_SHARED) -lm

all: link_static link_shared

clean:
	rm -rf $(BIN_DIR) $(OBJ_DIR) $(LIB_DIR) $(LIB_STATIC_DIR) $(LIB_SHARED_DIR)

