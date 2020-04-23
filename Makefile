CC=gcc
CFLAGS=-std=c99 -Wall -g
LDFLAGS=-g
BIN_DIR=bin/
TARGETS=$(BIN_DIR)now $(BIN_DIR)when $(BIN_DIR)period

all: $(TARGETS)

$(BIN_DIR)%: %.o
	$(CC) $(LDFLAGS) -o $@ $<

%.o: %.c
	$(CC) $(CFLAGS) -c -o $@ $<

clean:
	rm -f *.o

mrproper: clean
	rm -f $(TARGETS)
