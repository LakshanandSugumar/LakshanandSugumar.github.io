# Tooling

| Tool | Purpose |
| --- | --- |
| **Python discrete-event simulator** | Canonical implementation of the full protocol: slot self-allocation, anchor election, join contention with backoff, admission control and buddy takeover. Runs the paper experiments and produces CSVs and plots |
| **JavaScript port** | Bit-identical port of the simulator, cross-validated against Python across 21 scenarios |
| **Browser visualizer** | A logic-analyzer-style view of the schedule for inspecting slots, joins and takeovers, with no build step |
| **MPLAB X firmware** | Reference firmware for the PIC18F57Q83 nodes, in progress |
| **Measurement** | Fault-recovery bounds measured against the analytical model |
