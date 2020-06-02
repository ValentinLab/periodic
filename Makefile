# Compilation
CC=gcc
CFLAGS=-std=c99 -Wall -g -I include/
LDFLAGS=-g
LDLIBS=-L lib/ -l controlsyscall -l message

# Directories and files
BIN_DIR=bin/
LIB_DIR=lib/
LIB_H=include/controlsyscall.h include/message.h

# Targets
LIBS=$(LIB_DIR)libcontrolsyscall.so $(LIB_DIR)libmessage.so
TOOLS=$(BIN_DIR)now $(BIN_DIR)when $(BIN_DIR)period $(BIN_DIR)periodic $(BIN_DIR)launch_daemon

# Macro
MKDIR_BIN=mkdir -p bin

.PHONY: all
all: $(LIBS) $(TOOLS)

# ----- FILES -----

# now
$(BIN_DIR)now: now.o $(LIB_H)
	@$(MKDIR_BIN)
	$(CC) $(LDFLAGS) -o $@ $< $(LDLIBS)

# when
$(BIN_DIR)when: when.o $(LIB_H)
	@$(MKDIR_BIN)
	$(CC) $(LDFLAGS) -o $@ $< $(LDLIBS)

# period
$(BIN_DIR)period: period_main.o period_ds.o period_files.o $(LIB_H)
	@$(MKDIR_BIN)
	$(CC) $(LDFLAGS) -o $@ period_main.o period_ds.o period_files.o $(LDLIBS)

# periodic
$(BIN_DIR)periodic: periodic.o $(LIB_H)
	@$(MKDIR_BIN)
	$(CC) $(LDFLAGS) -o $@ $< $(LDLIBS)

#launch_daemon
$(BIN_DIR)launch_daemon: launch_daemon.o $(LIB_H)
	@$(MKDIR_BIN)
	$(CC) $(LDFLAGS) -o $@ $< $(LDLIBS)

%.o: %.c
	$(CC) $(CFLAGS) -c -o $@ $<

# ----- LIBRARIES -----

# libmessage.so
$(LIB_DIR)libmessage.so: $(LIB_DIR)src/message.o
	$(CC) $(LDFLAGS) -shared -o $@ $< -L lib/ -l controlsyscall

# libcontrolsyscall.so
$(LIB_DIR)libcontrolsyscall.so: $(LIB_DIR)src/controlsyscall.o
	$(CC) $(LDFLAGS) -shared -o $@ $<

$(LIB_DIR)src/%.o: $(LIB_DIR)src/%.c include/%.h
	$(CC) $(CFLAGS) -c -fPIC -o $@ $<

# ----- HOUSE CLEANING -----

.PHONY: clean mrproper

$(INCLUDE_DIR)libmessage.so: $(INCLUDE_DIR)message.o
	$(CC) -o $(INCLUDE_DIR)libmessage.so -shared $(INCLUDE_DIR)message.o

$(INCLUDE_DIR)message.o: $(INCLUDE_DIR)message.c
	$(CC) $(CFLAGS) -fPIC -o $@ -c $<

clean:
	rm -f *.o
	rm -f lib/src/*.o

mrproper: clean
	rm -f $(BIN_DIR)*
	rm -f $(LIB_DIR)*.so