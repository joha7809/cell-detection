CC = gcc
CFLAGS = -std=c99 -Wall
LDFLAGS = -lm
TARGET = main.out

SRCS := $(filter-out ./samples/% ./results_example/%, $(wildcard *.c))
OBJS := $(SRCS:.c=.o)

.PHONY: all clean run

all: $(TARGET)

$(TARGET): $(OBJS)
	$(CC) $(CFLAGS) $^ -o $@ $(LDFLAGS)

%.o: %.c
	$(CC) $(CFLAGS) -c $< -o $@

run: all
	./$(TARGET) example.bmp example_inv.bmp

clean:
	rm -f $(OBJS) $(TARGET)
