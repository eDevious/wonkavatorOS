CC = gcc
CFLAGS = -std=c17 -Wall -O2 -Istage0 -pthread
LDFLAGS = -lpthread
SRC = stage0/main.c stage0/cli.c stage0/hardware.c stage0/orchestrator.c stage0/net_scan.c
OBJ = $(SRC:.c=.o)
TARGET = wonkavator

all: $(TARGET)

$(TARGET): $(OBJ)
	$(CC) $(OBJ) -o $(TARGET) $(LDFLAGS)

%.o: %.c
	$(CC) $(CFLAGS) -c $< -o $@

clean:
	rm -f $(OBJ) $(TARGET)

run: $(TARGET)
	sudo ./$(TARGET)

.PHONY: all clean run
