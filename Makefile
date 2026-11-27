CC=gcc
CFLAGS=-std=c17 -Wall -O2 -Istage0
LDFLAGS=
SRC=$(wildcard stage0/*.c)
TARGET=wonkavator

all: $(TARGET)

$(TARGET): $(SRC)
	$(CC) $(CFLAGS) $(SRC) -o $(TARGET) $(LDFLAGS)

clean:
	rm -f $(TARGET) ai_placer.dat stage0/*.o

run: $(TARGET)
	./$(TARGET)

.PHONY: all clean run
