cat > Makefile << 'EOF'
CC=gcc
CFLAGS=-std=c17 -Wall -O2 -Istage0
SRC=$(wildcard stage0/*.c)

all:
	$(CC) $(CFLAGS) $(SRC) -o wonkavator

clean:
	rm -f wonkavator wonk.lock ai_placer.dat

.PHONY: all clean
EOF
