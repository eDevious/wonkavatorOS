CC = gcc
CFLAGS = -std=c17 -Wall -O2 -Istage0 -pthread
LDFLAGS = -lpthread
SRCS = stage0/main.c stage0/cli.c stage0/hardware.c
OBJS = $(SRCS:.c=.o)
TARGET = wonkavator
all: $(TARGET)
$(TARGET): $(OBJS)
	$(CC) $(OBJS) -o $(TARGET) $(LDFLAGS)
%.o: %.c
	$(CC) $(CFLAGS) -c $< -o $@
clean:
	rm -f stage0/*.o $(TARGET) manifest_*.txt manifest_*.csv
.PHONY: all clean
