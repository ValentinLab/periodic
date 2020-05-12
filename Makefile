CC=gcc
CFLAGS=-std=c99 -Wall -g -I include/
LDFLAGS=-g
LDLIBS=-L lib/ -l controlsyscall
BIN_DIR=bin/
LIB_DIR=lib/
LIB=$(LIB_DIR)libcontrolsyscall.so
TARGETS=$(BIN_DIR)now $(BIN_DIR)when $(BIN_DIR)period $(BIN_DIR)launch_daemon

all: $(TARGETS)

# ----- FILES -----

$(BIN_DIR)%: %.c $(LIB)
	$(CC) $(CFLAGS) $(LDLIBS) -o $@ $<

# ----- LIBRARIES -----

$(LIB_DIR)libcontrolsyscall.so: $(LIB_DIR)src/controlsyscall.o
	$(CC) $(LDFLAGS) -shared -o $@ $<

$(LIB_DIR)src/%.o: $(LIB_DIR)src/%.c include/%.h
	$(CC) $(CFLAGS) -c -fPIC -o $@ $<

# ----- HOUSE CLEANING -----

.PHONY: clean mrproper

clean:
	rm -f *.o
	rm -f lib/src/*.o

mrproper: clean
	rm -f $(BIN_DIR)*
	rm -f $(LIB_DIR)*.so