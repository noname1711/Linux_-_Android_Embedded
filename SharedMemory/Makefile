.PHONY: all clean posix-reader posix-writer systemv-reader systemv-writer systemv-key directories

# Directories
SRC_DIR = src
POSIX_DIR = $(SRC_DIR)/POSIX
SYSTEMV_DIR = $(SRC_DIR)/systemV
BIN_DIR = bin
IPC_KEY_FILE = /tmp/systemV_ipc_key

# Compiler settings
CC = gcc
CFLAGS = -Wall -Wextra -lrt

# Targets
TARGETS = posix-reader posix-writer systemv-reader systemv-writer

all: directories $(TARGETS)

directories:
	@mkdir -p $(BIN_DIR)

systemv-key:
	touch $(IPC_KEY_FILE)

# POSIX targets
posix-reader: directories
	$(CC) $(CFLAGS) -o $(BIN_DIR)/$@ $(POSIX_DIR)/reader.c
	@echo "Running POSIX reader..."
	$(BIN_DIR)/$@

posix-writer: directories
	$(CC) $(CFLAGS) -o $(BIN_DIR)/$@ $(POSIX_DIR)/writer.c
	@echo "Running POSIX writer..."
	$(BIN_DIR)/$@

# SystemV targets
systemv-reader: directories systemv-key
	$(CC) $(CFLAGS) -o $(BIN_DIR)/$@ $(SYSTEMV_DIR)/reader.c
	@echo "Running SystemV reader..."
	$(BIN_DIR)/$@

systemv-writer: directories systemv-key
	$(CC) $(CFLAGS) -o $(BIN_DIR)/$@ $(SYSTEMV_DIR)/writer.c
	@echo "Running SystemV writer..."
	$(BIN_DIR)/$@

# Clean 
clean:
	rm -rf $(BIN_DIR)
	@if [ -f $(IPC_KEY_FILE) ]; then \
		echo "Removing SystemV key file..."; \
		rm -f $(IPC_KEY_FILE); \
	fi
	@echo "Cleanup completed."