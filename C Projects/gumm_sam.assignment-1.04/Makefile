CC = gcc

CFLAGS = -Wall -Wextra -g 

OBJS = dungeon.o heap.o 

TARGET = start

all: $(TARGET)

$(TARGET): $(OBJS)
	$(CC) $(CFLAGS) -o $(TARGET) $(OBJS)

dungeon.o: dungeon.c dungeon.h 

heap.o: heap.c heap.h 

.PHONY: clean
clean:
	rm -f $(TARGET) $(OBJS)