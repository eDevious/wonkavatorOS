CC = gcc
CFLAGS = -std=c17 -Wall -O2 -Istage0 -D_CRT_SECURE_NO_WARNINGS
TARGET = wonkavator.exe
SRCS = stage0/boot.c stage0/cli.c stage0/orchestrator.c stage0/dma_driver.c stage0/wonk_sync.c stage0/ai_placer.c
OBJS = $(SRCS:.c=.o)

all: $(TARGET)

$(TARGET): $(OBJS)
    $(CC) $(OBJS) -o $(TARGET)

%.o: %.c
    $(CC) $(CFLAGS) -c $< -o $@

run: $(TARGET)
    ./$(TARGET)

clean:
    cmd /c "del /Q stage0\*.o $(TARGET) 2>NUL"
    cmd /c "del /Q wonk.lock ai_placer.dat 2>NUL"

.PHONY: all run clean
