cat > Makefile << 'EOF'
CC = gcc
CFLAGS = -std=c17 -Wall -O2 -Istage0
TARGET = wonkavator
SRCS = $(wildcard stage0/*.c)
OBJS = $(SRCS:.c=.o)

all: $(TARGET)

$(TARGET): $(OBJS)
	$(CC) $(OBJS) -o $(TARGET)

%.o: %.c
	$(CC) $(CFLAGS) -c $< -o $@

clean:
	rm -f stage0/*.o $(TARGET) wonk.lock ai_placer.dat

.PHONY: all clean
EOF
