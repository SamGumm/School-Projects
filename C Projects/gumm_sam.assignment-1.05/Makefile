CC = gcc

CFLAGS = -Wall -Wextra -g 

OBJS = dungeon.o heap.o 

TARGET = start

LDFLAGS = -lncurses

all: $(TARGET)

$(TARGET): $(OBJS)
	$(CC) $(CFLAGS) -o $(TARGET) $(OBJS) $(LDFLAGS)

dungeon.o: dungeon.c dungeon.h 

heap.o: heap.c heap.h 

.PHONY: clean
clean:
	rm -f $(TARGET) $(OBJS)