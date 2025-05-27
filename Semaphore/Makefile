.PHONY: all clean run_unnamed run_named

# Compiler and flags
CC = gcc
CFLAGS = -Wall -Wextra -pthread
LDFLAGS = -pthread

# Targets
TARGETS = unnamed_semaphore named_sem_writer named_sem_reader
SRC_DIR = src
BIN_DIR = bin

# Default target
all: $(addprefix $(BIN_DIR)/,$(TARGETS))

# Create bin directory if it doesn't exist
$(shell mkdir -p $(BIN_DIR))

# Compile rules
$(BIN_DIR)/unnamed_semaphore: $(SRC_DIR)/unnamed_semaphore.c
	$(CC) $(CFLAGS) $< -o $@ $(LDFLAGS)

$(BIN_DIR)/named_sem_writer: $(SRC_DIR)/named_sem_writer.c
	$(CC) $(CFLAGS) $< -o $@

$(BIN_DIR)/named_sem_reader: $(SRC_DIR)/named_sem_reader.c
	$(CC) $(CFLAGS) $< -o $@

# Run targets
run_unnamed: $(BIN_DIR)/unnamed_semaphore
	@echo "Running unnamed semaphore example..."
	@./$(BIN_DIR)/unnamed_semaphore

run_named: $(BIN_DIR)/named_sem_writer $(BIN_DIR)/named_sem_reader
	@echo "Running named semaphore example..."
	@./$(BIN_DIR)/named_sem_writer && ./$(BIN_DIR)/named_sem_reader

# Cleanup
clean:
	rm -rf $(BIN_DIR) shared.txt



