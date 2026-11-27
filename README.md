README_v0.4.md - "THE CHARTMAKER"
markdown
# WONKAVATOR OS v0.4 "THE CHARTMAKER"

*She sees. She remembers. She charts the course.*

---

## THE SHIP'S HISTORY

Hear me, ye scallywags.  
v0.1 to v0.3 gave her eyes and a voice. She could scan and shout.  
But a captain without a logbook is lost at sea.

v0.4 is the first hull to carry parchment.  
This be the Chartmaker. She takes a snapshot of the ship, carves it into a manifest,  
and on the next tide she can tell ye what changed, what broke, and what new cargo came aboard.

She does not yet command. She does not yet hunt.  
She observes. She records. She endures.

---

## CREW & CAPABILITIES

| Deck | Command | Purpose |
| --- | --- | --- |
| **Bridge** | `info` | Captain's report. Kernel, CPU, Memory at a glance |
| **Crow's Nest** | `scan` | Quick sweep. CPU, MEM, and all NET with IP/MAC |
| **Chart Room** | `manifest` | **FULL SURVEY**. CPU, MEM, NET, PCI, USB, DMI. Then saves to parchment |
| **Ship's Log** | `hosts` | List all manifests in the captain's cabin. `manifest_YYYYMMDD_HHMMSS.txt` |
| **Quartermaster** | `load` | Unfurl an old chart: `load manifest_20251127_030000.txt` |
| **Lookout** | `diff` | Compare this horizon to the last chart. "What be different, Mr. Jones?" |
| **Scribe** | `export` | Transcribe the last manifest to CSV for the admiralty ashore |
| **Abandon Ship** | `exit` | Furl the sails. Secure the logbooks. |

---

## WHAT MAKES v0.4 SPECIAL

1.  **Persistence**: First version to save state. Every `manifest` creates a timestamped file.
2.  **Comparison**: `diff` lets you see drift. New USB device? New PCI card? She'll tell ye.
3.  **Portability**: `export` to CSV so the land-lubbers in accounting can read it.
4.  **Fullness**: `manifest` is the first true broadside. It hits PCI, USB, and DMI, not just CPU.

She is not yet alive. But she has a memory. And that is the first step to a soul.

---

## BUILDING THE CHARTMAKER

Ye need only a quill and ink:
```bash
sudo apt install gcc make dmidecode lspci usbutils
Run the Makefile incantation, then:
bash
make clean && make
sudo ./wonkavator
Note: dmidecode, lspci, lsusb must be installed or some charts will be blank
FIRST VOYAGE ORDERS
A proper captain always takes 3 readings:
javascript
manifest    // Take the first chart. Name it.
scan        // Quick check a bell later
manifest    // Take a second chart
diff        // "What changed between them?"
hosts       // See all your charts
export      // Send a copy to port
CAPTAIN'S NOTES
v0.4 is a quiet ship. She does not spawn daemons. She does not watch processes.
She is a librarian with a spyglass.
But mark my words: every great admiral starts as a chartmaker.
From memory comes learning. From learning comes the Bull.
"Steady as she goes. Log everything."