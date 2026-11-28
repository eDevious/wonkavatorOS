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

WONKAVATOR v0.9 - DUDE RANCH BUILD GUIDE
From bare dirt to seaworthy vessel
1. PREPARE THE RANCH - DEPENDENCIES
Make sure your ranch has the right tools in the shed.
For Debian/Ubuntu:
bash
sudo apt update
sudo apt install build-essential gcc make git dmidecode pciutils usbutils
For Arch:
bash
sudo pacman -S base-devel git dmidecode pciutils usbutils
For Fedora:
bash
sudo dnf groupinstall "Development Tools"
sudo dnf install git dmidecode pciutils usbutils
dmidecode, lspci, lsusb are needed for the fingerprint command to work proper.
2. LAY THE FOUNDATION - GET THE CODE
bash
cd ~
git clone <your-repo-url> wonkavatorOS
cd wonkavatorOS
Or if you're building from our 6-file dump:
bash
mkdir -p ~/wonkavatorOS/stage0
cd ~/wonkavatorOS
Then paste all 6 files: Makefile, main.c, cli.c, boot.h, hardware.c, net_scan.c, hardware_fingerprint.c, orchestrator.c
3. INSPECT THE LUMBER - FILE TREE CHECK
Your ranch should look like this:
javascript
wonkavatorOS/
├── Makefile
└── stage0/
    ├── boot.h
    ├── main.c
    ├── cli.c
    ├── hardware.c
    ├── net_scan.c
    ├── hardware_fingerprint.c
    └── orchestrator.c
4. RAISE THE BARN - COMPILE
Run the build from the ranch root:
bash
make clean
make
Expected Output:
javascript
gcc -std=c17 -Wall -O2 -Istage0 -pthread -c -o stage0/main.o stage0/main.c
gcc -std=c17 -Wall -O2 -Istage0 -pthread -c -o stage0/cli.o stage0/cli.c
...
gcc stage0/main.o ... -o wonkavator -lpthread
[BUILD] Wonkavator v0.9 ready
If you see error: unknown type name ‘va_list’ you forgot #include <stdarg.h> in boot.h. We fixed that in v0.9.
5. BRAND THE CATTLE - FIRST RUN
You need sudo for hardware access:
bash
sudo ./wonkavator
You should see:
javascript
[WONKAVATOR] v0.9 STOPPABLE SCAN Booting...
[WONKAVATOR] Running initial manifest...
[LEVIATHAN] v0.9 Type 'help'

[LEVIATHAN] > 
6. SHAKEDOWN CRUISE - TEST COMMANDS
Run these in order to verify the ship:
bash
[LEVIATHAN] > help
[LEVIATHAN] > fingerprint
[LEVIATHAN] > scanport 127.0.0.1
[LEVIATHAN] > stop
[LEVIATHAN] > exit
Then check the ship's log:
bash
cat wonkavator.log
7. TROUBLESHOOTING THE FENCE LINE
Problem
Fix
make: command not found
Install build-essential or base-devel
permission denied on ./wonkavator
chmod +x wonkavator
dmidecode: command not found
sudo apt install dmidecode
Scan hangs
Type stop to abort. All scans are interruptible in v0.9
wonkavator.log is empty
Check you have write permission in the dir
8. RANCH RULES FOR v0.9
One command at a time: Don't run watch and scanport together yet
Log everything: If something weird happens, check wonkavator.log first
No version bumps: We stay on v0.9 until the captain says so
We do not ship half-baked features.
We build, we test, we log, we sail again.
v0.9 is the hull. Strong. Watertight. Ready.
Next port is yours to choose, boss.
Fair winds and following seas.
~ The Wonkbox
