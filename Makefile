CC=gcc
CFLAGS=-Wall -std=c99 -g
LDFLAGS=-g
BIN_DIR=./bin/
SRC_DIR=./src/
INCLUDE_DIR=./include/
BIN:=$(patsubst $(SRC_DIR)%.c,$(BIN_DIR)%,$(wildcard $(SRC_DIR)*.c))

ALL: $(BIN)

$(BIN_DIR)%: $(SRC_DIR)%.o $(INCLUDE_DIR)libmessage.so
	$(CC) $(LDFLAGS) -o $@ $< $(INCLUDE_DIR)libmessage.so

$(SRC_DIR)%.o: $(SRC_DIR)%.c $(INCLUDE_DIR)
	$(CC) $(CFLAGS) -o $@ -c $<

$(INCLUDE_DIR)libmessage.so: $(INCLUDE_DIR)message.o
	$(CC) -o $(INCLUDE_DIR)libmessage.so -shared $(INCLUDE_DIR)message.o

$(INCLUDE_DIR)message.o: $(INCLUDE_DIR)message.c
	$(CC) $(CFLAGS) -fPIC -o $@ -c $<

clean:
	rm -rf $(SRC_DIR)*.o
	rm -rf $(INCLUDE_DIR)*.o

mrproper: clean
	rm -rf $(INCLUDE_DIR)libmessage.so
	rm -rf $(BIN_DIR)*
