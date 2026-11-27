README.md - v0.9 "NETWORK BULL TICK"

# WONKAVATOR OS v0.9 "NETWORK BULL TICK"
### *One Brain. Eleven Ticks. Infinite Fleet.*

---

## I. THE ADMIRAL'S LOG

Gather round, ye dogs. This be the tale of v0.9.

v0.1 gave her eyes. v0.4 gave her memory. v0.7 gave her claws. v0.8 gave her a crew of 11 QEMU daemons lashed to the hull.  
But v0.9... v0.9 cuts the chains and raises the signal flags.

She is no longer captain of one ship.  
She is Admiral of a Fleet.

Through QMP and TCP, she whispers orders across the network. She measures the wind and waves to Melbourne, Sydney, and the bull in the next rack.  
And on each `ntick`, she sends the work to the hull that pulls the hardest.

This be WonkavatorOS v0.9. She latches, she learns, and now... she commands.

---

## II. THE FULL MANIFEST - ALL DECKS v0.1 TO v0.9

### **DECK 1: THE BRIDGE - Core Scans v0.1-v0.6**
The foundation. The spyglass and the compass.
- `info`       : Captain's report. Kernel, Hostname, Arch
- `scan`       : Quick sweep. CPU, Memory, All NET interfaces with IP/MAC
- `manifest`   : Full broadside survey. CPU, MEM, NET, PCI, USB, DMI. Saves to log
- `hosts`      : The Ship's Log. List all `manifest_*.txt` in the cabin
- `load`       : Unfurl an old chart: `load manifest_xxx.txt`
- `diff`       : The Lookout. "What has changed since last manifest?"
- `export`     : The Scribe. Transcribe last manifest to CSV
- `watch`      : Night Watch. Auto-scan every 5 seconds
- `stop`       : Cease the Night Watch

### **DECK 2: THE HUNTER'S GALLEY - Tools v0.7**
She grew teeth and learned to hunt.
- `probe`      : Deep Sounding. Plumb /proc and /sys for hidden truths
- `parasite`   : Barnacles. Attach and monitor top 3 CPU processes every 2s
- `pstop`      : Boatswain's whistle. Manual stop for parasite only
- `tracer`     : Chronometer. Mark function latency with nanosecond precision

### **DECK 3: THE CAPTAIN'S QUARTERS - Orchestration v0.8**
She got a crew and a brain.
- `boot`       : Shipwright. Adaptive boot. Detects x86_64/ARM64. Loads KVM, SVM, VT-x
- `chip`       : Armory. Chip-Sense. Reads CPU/GPU/NIC. Biases orchestrator: AMD favors CPU, Intel favors IO
- `tick`       : Captain's Order. Send CPU/NET/IO job to 11 local QEMU nodes. Learns via EMA
- `herd`       : Muster. Show all 11 nodes. PID, CPU Score, NET Score, IO Score, Jobs Done

### **DECK 4: THE ADMIRALTY - Fleet Command v0.9**
The chains are cut. The sea is ours.
- `qmp`        : Signalman. Send QEMU Monitor Protocol command to fastest local node
- `addbull`    : Cartographer. `addbull 192.168.1.20 9000` - Add a remote Wonkavator to the fleet
- `ntick`      : Admiral's Order. **Distributed Tick**. Ping local and remote. Route job to fastest hull
- `scanip`     : Admiralty Scan. Adaptive /24 subnet discovery. Finds all hosts
- `scanport`   : Gunner. `scanport 192.168.1.5` - Verbose port probe: 22, 80, 443, 9000, etc
- `addauto`    : Press Gang. Auto-add any discovered host with port 9000 open

- `exit`       : Abandon Ship. Secure all logs and stand down

---

## III. THE BULL TICK DOCTRINE

This be how the Admiral thinks:

1.  **Adaptive Boot**: On launch, `boot` sounds the hull. Is it x86_64 or ARM64? Load the right microcode.
2.  **Chip-Sense**: On `chip`, she reads the iron. NVIDIA? AMD? Xeon? She remembers.
3.  **Local Tick**: `tick` sends work to 11 local QEMU nodes. Latency is measured. Scores are updated in `.orch_memory`.
4.  **Fleet Tick**: `ntick` extends the measure. It pings remote bulls added via `addbull`. It compares local latency vs remote latency.
5.  **The Routing**: The next job goes to the absolute fastest. No politics. No favorites. Only speed.

Her memory persists across voyages in `.orch_memory`. Delete it, and she becomes a newborn.

---

## IV. BUILDING THE FLAGSHIP

Ye shall need iron, tar, and the beast itself.

**Requirements:**
```bash
sudo apt install gcc make qemu-system-x86_64 dmidecode pciutils usbutils
Forge the vessel:
bash
# 1. Create the Makefile
cat > Makefile << 'EOF'
CC = gcc
CFLAGS = -std=c17 -Wall -O2 -Istage0 -pthread
LDFLAGS = -lpthread
SRCS = stage0/main.c stage0/cli.c stage0/hardware.c stage0/orchestrator.c stage0/net_scan.c
OBJS = $(SRCS:.c=.o)
TARGET = wonkavator
all: $(TARGET)
$(TARGET): $(OBJS)
	$(CC) $(OBJS) -o $(TARGET) $(LDFLAGS)
%.o: %.c
	$(CC) $(CFLAGS) -c $< -o $@
clean:
	rm -f stage0/*.o $(TARGET) manifest_*.txt manifest_*.csv .orch_memory
.PHONY: all clean
EOF

# 2. Build
make clean && make

# 3. Sail
sudo ./wonkavator
Note: Must be root for dmidecode and full QEMU control
V. THE ADMIRAL'S FIRST VOYAGE
A proper shakedown cruise:
javascript
boot        // Sound the hull. Load drivers
chip        // Read the guns. Verify GPU
manifest    // Take first chart
tick        // Command local herd
herd        // Muster and see scores
scanip      // Find other ships on the sea
addbull 10.0.0.5 9000 // Add one to the fleet
ntick       // Command the entire fleet
qmp         // Send signal to fastest
parasite    // Set the barnacles
pstop       // Call them off
exit        // Secure for night
VI. FILES OF IMPORTANCE
File
Purpose
manifest_*.txt
Parchment. Saved full system surveys
manifest_*.csv
Admiralty report. For land-lubbers
.orch_memory
The Brain. Stores EMA scores for all 11 nodes
VII. THE FUTURE - CHARTED WATERS
The Admiral has spoken of v1.0:
Encrypted Comms: TLS between bulls. No more open signals.
Auto-Discovery: Bonjour/mDNS. Bulls find each other.
GPU Offload: Send CUDA/ROCm jobs to the bull with the strongest GPU.
QMP Storage: Live migrate ticks between hulls.
For now, the fleet is ours.
"From one bull to a thousand.
From one ship to an empire.
All ahead full."
The Admiral, v0.9