# WONKAVATOR OS v0.7 LEVIATHAN
# The Shipwright's Build System

CC      := gcc
CFLAGS  := -std=c17 -Wall -O2 -Istage0 -pthread
LDFLAGS := -lpthread
SRC     := $(wildcard stage0/*.c)
TARGET  := wonkavator

all: $(TARGET)

$(TARGET): $(SRC)
	@echo "⚓ Forging WonkavatorOS v0.7 LEVIATHAN..."
	$(CC) $(CFLAGS) $^ -o $@ $(LDFLAGS)
	@echo "✅ Shipwright complete. Vessel ready: ./$(TARGET)"

run: $(TARGET)
	@echo "⚓ Hoisting the colors..."
	sudo ./$(TARGET)

clean:
	@echo "🧹 Swabbing the decks..."
	rm -f $(TARGET) 
	rm -f manifest_*.txt hotplug_*.txt ai_placer.dat last_manifest.snap
	rm -f /tmp/wonk_diff
	@echo "✅ Decks clean"

deps:
	@echo "📦 Loading stores..."
	sudo apt update && sudo apt install -y build-essential pciutils usbutils dmidecode

.PHONY: all clean run deps
