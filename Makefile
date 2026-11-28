CC=gcc
CFLAGS=-std=c17 -Wall -O2 -Istage0 -pthread
SRC=stage0/main.c stage0/cli.c stage0/hardware.c stage0/orchestrator.c stage0/net_scan.c stage0/hardware_fingerprint.c
OBJ=$(SRC:.c=.o)
TARGET=wonkavator

all: $(TARGET)
	@echo "[BUILD] Wonkavator v0.9.2 DEEP FINGERPRINT ready"

$(TARGET): $(OBJ)
	$(CC) $(OBJ) -o $(TARGET) -lpthread

clean:
	rm -f $(OBJ) $(TARGET)
	@echo "[CLEAN] Done"

.PHONY: all clean
