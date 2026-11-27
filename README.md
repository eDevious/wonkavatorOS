README_v0.7.md - "THE LEVIATHAN AWAKENS"
markdown
# WONKAVATOR OS v0.7 "THE LEVIATHAN"

*From the depths she rises. She watches. She hunts.*

---

## THE SHIP'S HISTORY

Hearken ye, for v0.7 be the first hull to grow a spine.  
Before this, she was but a lantern and a spyglass. Now she is a warship.

v0.7 consolidates all logs from v0.1 to v0.6 and adds the forbidden tools:
Process parasites, kernel tracers, and deep probes into the belly of the machine.

She no longer just charts the stars. She listens to the creaking of the hull.

---

## CREW & CAPABILITIES

| Deck | Command | Purpose |
| --- | --- | --- |
| **Bridge** | `info` | Ship's manifest. Kernel, CPU, Memory |
| **Crow's Nest** | `scan` | Quick sweep of the horizon. CPU/MEM/NET |
| **Chart Room** | `manifest` | Full broadside survey. Saves to log for the admiralty |
| **Ship's Log** | `hosts` | List all saved manifests |
| **Quartermaster** | `load` | Hoist a previous log: `load manifest_xxx.txt` |
| **Lookout** | `diff` | Compare this tide to the last. What has changed? |
| **Scribe** | `export` | Chart the data to CSV for land-lubbers |
| **Night Watch** | `watch` | Auto-scan every 5 seconds. Never sleep |
| **Deep Sounding** | `probe` | Plumb the depths of /proc and /sys |
| **Parasite** | `parasite` | Attach barnacles to processes. Monitor top 3 CPU hogs |
| **Chronometer** | `tracer` | Mark time with a sextant. Log function latency |
| **Abandon Ship** | `exit` | All hands. Cease all operations |

---

## BUILDING THE LEVIATHAN

Ye shall need iron and tar:
```bash
sudo apt install gcc make
Run the Makefile incantation, then:
bash
make clean && make
sudo ./wonkavator
First launch will run a full manifest. The Leviathan learns the shape of her hull.
THE DOCTRINE OF v0.7
This be not a tool. This be a living thing.
probe to feel the bones. parasite to taste the blood. tracer to hear the heartbeat.
She is v0.7. And she is hungry.
"Steady as she goes. All ahead slow."
javascript

---

