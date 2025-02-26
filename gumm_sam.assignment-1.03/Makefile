CC = gcc

CFLAGS = -Wall -Wextra -g

OBJS = main.o heap.o

TARGET = main

all: $(TARGET)

$(TARGET): $(OBJS)
	$(CC) $(CFLAGS) -o $(TARGET) $(OBJS)

main.o: main.c heap.h main.h 
	$(CC) $(CFLAGS) -c main.c 

heap.o: heap.c heap.h 
	$(CC) $(CFLAGS) -c heap.c

.PHONY: clean
clean:
	rm -f $(TARGET) $(OBJS)