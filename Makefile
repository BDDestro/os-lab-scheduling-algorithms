CC = gcc
CFLAGS = -std=c99 -Wall -Wextra -O2
BIN = bin
TARGETS = $(BIN)/fcfs $(BIN)/sjf $(BIN)/srtf $(BIN)/round_robin $(BIN)/priority

all: $(TARGETS)

$(BIN):
	mkdir -p $(BIN)

$(BIN)/%: %.c | $(BIN)
	$(CC) $(CFLAGS) $< -o $@

clean:
	rm -rf $(BIN)

.PHONY: all clean
