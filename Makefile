CC=gcc
CFLAGS=-std=c99 -Wall -g -I include/
LDFLAGS=-g
LDLIBS=-L lib -l controlsyscall
BIN_DIR=bin/
TARGETS=$(BIN_DIR)now $(BIN_DIR)when $(BIN_DIR)period

all: $(TARGETS)

# ----- FILES -----

# now.c

$(BIN_DIR)now: now.o
	$(CC) $(LDFLAGS) -o $@ $<

now.o: now.c
	$(CC) $(CFLAGS) -c -o $@ $<

# when.c

$(BIN_DIR)when: when.o
	$(CC) $(LDFLAGS) -o $@ $<

when.o: when.c
	$(CC) $(CFLAGS) -c -o $@ $<

# periodic.c

$(BIN_DIR)periodic: periodic.o lib/libcontrolsyscall.so
	$(CC) $(LDFLAGS) $(LDLIBS) -o $@ $<

periodic.o: periodic.c
	$(CC) $(CFLAGS) -c -o $@ $<

# period.c

$(BIN_DIR)period: period.o lib/libcontrolsyscall.so
	$(CC) $(LDFLAGS) $(LDLIBS) -o $@ $<

period.o: period.c
	$(CC) $(CFLAGS) -c -o $@ $<

# ----- LIBRARIES -----

# libcontrolsyscall.so

lib/libcontrolsyscall.so: lib/src/controlstream.o
	$(CC) $(LDFLAGS) -shared -o $@ $<

lib/src/controlstream.o: lib/src/controlstream.c include/controlstream.h
	$(CC) $(CFLAGS) -c -fPIC -o $@ $<

# House cleaning

clean:
	rm -f *.o
	rm -f lib/src/*.o

mrproper: clean
	rm -f $(TARGETS)
	rm -f lib/*.so