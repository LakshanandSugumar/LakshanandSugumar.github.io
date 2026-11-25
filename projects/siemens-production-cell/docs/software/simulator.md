# Mock PLC and cell simulator

`python/cellsim` stands in for the S7-1500 until the TIA Portal project exists. It has two layers:

| Module | Role |
| --- | --- |
| `sequences.py` | The three station sequences as data: steps, actions, transitions, interlocks, nominal times, supervision limits, fault codes and texts. Single source of truth for the simulator, `docs/02_sequences.md` and the dashboard's fault texts. |
| `tags.py` | The OPC UA tag contract: DB, name, PLC type, access, description. Shared by the server, the collector and `docs/05_opcua_interface.md`. |
| `model.py` | Time-stepped model of the cell (pure Python, no I/O, deterministic for a seed). |
| `server.py` | asyncua OPC UA server that runs the model and publishes the tags. |
| `cmd.py` | Command-line HMI: start, stop, reset, mode, fault injection. |

## How the model works

Every tick (default 0.1 s of PLC time) the model advances all three station sequences side by side.

- **Timed steps** last their nominal time with ±10 % jitter. Each has a supervision limit; if a step is *jammed* it never completes and faults once the limit is reached, exactly like S7-GRAPH supervision (V).
- **Wait steps** (starved or blocked) wait on a handshake and are never supervised.
- **Handshakes** are two one-part buffers: station 1 → 2 and station 2 → 3. A station only hands a part on when the next buffer is empty.
- **Inspection**: after reading the part type, station 1 rejects a share of parts (`--reject-rate`, default 5 %) through the pusher branch.
- **Faults**: any timed step can jam at random (`--fault-prob`, default 0.2 % per step execution), or on demand with `cmd inject <station>`. A fault holds the whole cell until reset.
- **Reset**: by default a simulated operator resets after a random repair time (exponential, mean 45 s, minimum 5 s). With `--manual-reset` faults wait for `cmd reset`. After reset the faulted step restarts, like GRAPH `ACK_EF`.
- **Time accounting** matches `FB_Production`: PlannedTime counts in Auto while started (faults included), RunTime counts only when no fault is active.

## Server options

| Option | Default | Meaning |
| --- | --- | --- |
| `--endpoint` | `opc.tcp://0.0.0.0:4840` | OPC UA endpoint |
| `--speed` | 10 | Simulation speed vs wall clock |
| `--seed` | 1 | Random seed (same seed, same run) |
| `--fault-prob` | 0.002 | Jam chance per timed step |
| `--reject-rate` | 0.05 | Share of parts rejected at inspection |
| `--manual-reset` | off | Faults wait for `cmd reset` |
| `--stopped` | off | Start with the cell stopped (send `cmd start`) |

## Commands

```bash
python -m cellsim.cmd start | stop | reset
python -m cellsim.cmd mode 2        # 1 Auto, 2 Manual, 3 Setup (refused while running)
python -m cellsim.cmd inject 2      # jam the current timed step of station 2
```

`cmd start/stop/reset/mode` also work against the real PLC, because they write `DB_HMI`; `inject` exists only in the mock (`DB_Sim`).

## Known simplifications

- One fault holds the whole cell; a real line often lets other stations empty out.
- Sensors are not modelled individually; a step is either on time, jittered, or jammed.
- No Manual-mode motion: in Manual the model just stops counting planned time.
- Right after a fault on station 3, a part already waiting at station 2 can give one short cycle-time reading (about 2.4 s). A real PLC measuring time between good parts would record the same.
