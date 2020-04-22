CC=gcc
CFLAGS=-Wall -std=c99 -g
LDFLAGS=-g
BIN_DIR=./bin/
SRC_DIR=./src/
BIN:=$(patsubst $(SRC_DIR)%.c,$(BIN_DIR)%,$(wildcard $(SRC_DIR)*.c))

ALL: $(BIN)

$(BIN_DIR)%: %.o
	$(CC) $(LDFLAGS) -o $(SRC_DIR)$@ $<

%.o: $(SRC_DIR)%.c
	$(CC) $(CFLAGS) -o $(SRC_DIR)$@ -c $<

clean:
	rm -rf $(SRC_DIR)*.o

mrproper: clean
	rm -rf ./bin/*
