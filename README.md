markdown
# WONKAVATOR OS v0.6 [FULLSCAN]
### *"She Sails The Digital Seas And Boards Every Port"*

       STAGE0 BOOTLOADER - GODLY EDITION v0.6
javascript

---

## ⚓ THE SHIP'S LOG

**Wonkavator OS v0.6** be a bare-metal stage0 bootloader with an AI Placement Bosun and a Parasitic Lookout in the crow's nest.  
She don't just boot. She **SCANS THE HORIZON** and **BOARDS EVERY SINGLE PORT** on any IP ye point her at.

This be the FULLSCAN edition. She will ping, probe, and plunder ports 1 through 65535. No mercy.

### **FEATURES OF THIS VESSEL**
-  **[DMA]** 3 Cargo Channels initialized at `0x50000000` for haulin' memory
-  **[AI PLACEMENT]** The Bosun learns which node sails fastest for each ISA/Kernel. Saved to `ai_placer.dat`
-  **[PARASITIC]** The Lookout fires real ICMP cannons and boards every TCP port with a cutlass
-  **[ORCHESTRATOR]** Deploys workloads across the fleet with GPIO sync pulses
-  **[CLI]** A proper captain's wheel. Type commands and she obeys

---

## 🗺️ CHARTS & RIGGING - BUILD INSTRUCTIONS

Ye need a Debian-class galleon to build this beast.

### **1. MUSTER THE CREW - DEPENDENCIES**
```bash
sudo apt update
sudo apt install build-essential netcat-openbsd -y
netcat-openbsd be required for the port-boarding operations.
2. RAISE THE JOLLY ROGER - COMPILE
bash
cd ~/wonkavatorOS
make clean
make
If the seas be calm, ye will see no errors.
3. SET SAIL
bash
./wonkavator
Ye should see:
javascript
Wonkavator OS Stage0 v0.6 [FULLSCAN]
[AI] No previous placement data
[DMA] 3 channels initialized at 0x50000000
Target: 192.168.1.100. Usage: parasite 192.168.1.50

wonk>
🧭 THE CAPTAIN'S WHEEL - COMMANDS
Command
Description
help
Show all available orders
probe
Sound the depths. 1 target node online
status
Ship's status report
deploy --workers=N --time=S
Send N sailors to work for S seconds. AI Bosun logs the times
ai-show
Read the Bosun's logbook of fastest nodes
ai-reset
Burn the logbook and start fresh
parasite [IP]
THE MAIN GUN Board target IP. Full 1-65535 TCP scan + Ping + Latency
hibernate
Save the Bosun's log and scuttle the shell
🏴‍☠️ FIRING THE MAIN GUN - PARASITE SCAN
This be what v0.6 be famous for. She will board EVERY. SINGLE. PORT.
Example Broadside:
javascript
wonk> parasite 192.168.1.50
Example Cannon Fire:
javascript
[PARASITE v0.6] FULL 1-65535 TCP SCAN ON 192.168.1.50
[PARASITE] =========================================
[1/3] PING... 3ms ALIVE
[2/3] SCANNING PORTS 1-65535
  Progress: 50%
  [+] PORT 22 OPEN
  [+] PORT 80 OPEN
  [+] PORT 443 OPEN
  Found 3 open ports
[3/3] CPU LATENCY... 9ms
[PARASITE] SCORE:0 PORTS:3
[PARASITE] TARGET CLEAR
SCORING SYSTEM
SCORE >= 3 = BLACKLISTED - This ship be a pirate. Avoid.
SCORE < 3 = CLEAR - Fair winds and following seas
SCORE increases for: Host Down, Slow Ping >50ms, Slow CPU >15ms, Too many open ports >50
⚠️ ARTICLES OF WAR
ONLY BOARD SHIPS YE OWN - Full port scans be illegal on waters ye don't own. Don't get keelhauled by the Admiralty.
SHE BE SLOW - 65535 ports with 0.5s timeout = ∼5 minutes per target. This be by design. We be thorough.
CHANGE TARGET IP - Edit boot.c line strcpy(g_node_health[0].ip, "192.168.1.100"); to set yer default port of call.
🔧 SHIP'S MANIFEST - FILE STRUCTURE
javascript
wonkavatorOS/
├── Makefile
└── stage0/
    ├── boot.h          # Ship's charter and global structs
    ├── boot.c          # The Helm. Initializes the fleet
    ├── cli.c           # The Captain's Wheel
    ├── orchestrator.c  # Assigns sailors to tasks
    ├── ai_placer.c     # The Bosun's Logbook
    ├── dma.c           # Cargo Hold Channels
    └── parasitic.c     # The Lookout + Boarding Party v0.6 FULLSCAN
🛠️ TROUBLESHOOTING IN A STORM
Segmentation fault
Scuttle the old logbook: rm ai_placer.dat then make clean && make
Found 0 open ports
Target be firewalled, offline, or cleaner than a nun's deck. Try parasite 127.0.0.1 after starting ssh
command not found: nc
sudo apt install netcat-openbsd
⚔️ GODLY v0.6 CHANGELOG
v0.6 - Parasitic Lookout now boards ALL 65535 ports. Verbose. Nautical. Godly.
v0.5 - AI Placement Bosun added
v0.1 - First launch. She floated.
"A ship in harbor is safe, but that is not what ships are built for.
A Wonkavator in harbor is just wastin' port scans."
- Captain Wonka, 2026
🏴‍☠️ Fair winds, super bean.
javascript

### **INSTALL IT**
```bash
cd ~/wonkavatorOS
cat > README.md <<'EOF'
[paste the whole thing above]
EOF
