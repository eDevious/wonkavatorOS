markdown
# WONKAVATOR OS
## The Leviathan of Local Orchestration
### Version: v0.9 - "Steady As She Goes"
.--.
/
| ⚓ | WONKAVATOR v0.9
__/
~
javascript

---

### **SHIP'S LOG**

Wonkavator is not a tool.  
Wonkavator is the ship.  

Built in C17, forged with `-Wall -O2`, and crewed by pthread sailors, Wonkavator is a local-first orchestration daemon designed to map, probe, fingerprint, and command the devices in your harbor.

This is v0.9. We sail slow. We sail deliberate. No cowboy jumps.

---

### **SHIP SPECIFICATIONS**

| Component | Detail |
| --- | --- |
| **Language** | C17, GCC |
| **Build System** | Make |
| **Threads** | pthreads for watch/parasite/scan |
| **Log** | `wonkavator.log` - Ship's Black Box |
| **Home Port** | `~/wonkavatorOS/` |
| **Binary** | `./wonkavator` |

---

### **CREW QUARTERS - FILE MANIFEST**

1.  `main.c` - The Captain's Wheel. Boots the ship and drops into the CLI.
2.  `boot.h` - The Ship's Articles. All declarations, logging, and shared charts.
3.  `cli.c` - The Helm. Parses orders and commands the crew.
4.  `hardware.c` - The Engineer. Runs manifests, probes, and system snapshots.
5.  `net_scan.c` - The Lookout. Pings the horizon and scans ports 1-65535.
6.  `hardware_fingerprint.c` - The Bosun. Takes DMI, PCI, USB, and MCU readings.
7.  `orchestrator.c` - The First Mate. Manages the herd of networked nodes.

---

### **SETTING SAIL - BUILD INSTRUCTIONS**

From your home port:
```bash
cd ~/wonkavatorOS
make clean
make
If the sea is calm, you will see 6 objects compile and link into one binary:
javascript
gcc ... -o wonkavator -lpthread
To launch:
bash
sudo ./wonkavator
Root is required for full hardware and raw socket access.
THE SHIP'S BELL - AVAILABLE COMMANDS
Type help once aboard.
Navigation & Recon
scan - Quick scan of CPU/Memory
manifest - Full system manifest. Host, CPU, RAM, Disks, Network
probe - Deep system probe. Kernel and OS details
info - Print host, cpu, and memory info
Cartography
hosts - List saved snapshots
load <name> - Load a snapshot
diff - Diff current state vs last snapshot
export - Export manifest to manifest.json
The Crow's Nest - Networking
scanip - Adaptive network discovery. Finds all hosts on /24
scanport [IP] - Full TCP 1-65535 scan. No IP = "CARPET BOMB" entire subnet
stop - ABORT any running scan. The emergency brake
The Bosun's Fingerprint Kit
fingerprint - Local DMI, PCI, USB, Serial. Finds Arduinos, ESPs, STM32s
fp <IP> - Remote firmware banner grab on target IP
The Engine Room
watch - Auto-scan every 5 seconds
parasite - Background monitor thread
tick - Orchestrator tick
herd - Show networked nodes
addbull <IP> <PORT> - Add node to the herd
ntick - Network tick, pings all herd members
qmp - Send QMP command to best node
Ship's Orders
exit - Secure all hatches and shutdown
SHIP'S ARTICLES - DESIGN PHILOSOPHY v0.9
Stoppable: Every long-running operation checks g_scanning. You are always in command.
Logged: Everything goes to stdout AND wonkavator.log
Local First: No cloud. No telemetry. This ship sails in your waters only.
Verbose: We don't do quiet. If something happens, the ship tells you.
KNOWN SHOALS
scanport ALL on a /24 will take time. Use stop if needed.
fingerprint uses dmidecode and lspci. Run as root.
Network herd is in-memory only for v0.9. Does not persist.
CAPTAIN'S ORDERS
We do not chase version numbers.
We do not ship half-baked features.
We build, we test, we log, we sail again.
v0.9 is the hull. Strong. Watertight. Ready.
Next port is yours to choose, boss.
Fair winds and following seas.
~ The Wonkbox
