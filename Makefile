CC=gcc
CFLAGS=-Wall -std=c99 -g
LDFLAGS=-g
BIN_DIR=./bin/
SRC=$(BIN_DIR)now $(BIN_DIR)when

ALL: $(SRC)

$(BIN_DIR)%: %.o
	$(CC) $(LDFLAGS) -o $@ $<

%.o: %.c
	$(CC) $(CFLAGS) -o $@ -c $<

clean:
	rm -rf *.o

mrproper: clean
	rm -rf ./bin/*
