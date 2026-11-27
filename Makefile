CC=gcc
CFLAGS=-std=c17 -Wall -O2 -Istage0
SRC=$(wildcard stage0/*.c)
TARGET=wonkavator

all: $(TARGET)

$(TARGET): $(SRC)
	$(CC) $(CFLAGS) $(SRC) -o $(TARGET)

clean:
	rm -f $(TARGET) wonk.lock ai_placer.dat stage0/*.o

run: $(TARGET)
	./$(TARGET)

.PHONY: all clean run
