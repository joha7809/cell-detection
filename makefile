CC = gcc
CFLAGS = -std=c99 -Wall
TARGET = main.out

SRCS := $(filter-out ./samples/% ./results_example/%, $(wildcard *.c))
OBJS := $(SRCS:.c=.o)

.PHONY: all clean run

all: $(TARGET)

$(TARGET): $(OBJS)
	$(CC) $(CFLAGS) $^ -o $@

%.o: %.c
	$(CC) $(CFLAGS) -c $< -o $@

run: all
	./$(TARGET) samples/medium/2MEDIUM.bmp example_inv.bmp

clean:
	rm -f $(OBJS) $(TARGET)
