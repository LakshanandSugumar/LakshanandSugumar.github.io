# Results

All results are protocol-level simulation results from the canonical Python simulator.

| Measurement | Result |
| --- | --- |
| Convergence, 63 nodes | Collision-free schedule within **88 frames**, worst case |
| Peer (buddy) takeover after a node fails | **3 frames**, against an analytical bound of **4** |
| Experiment sweep | **1,436** simulation runs with **zero** slot collisions and **zero** replica disagreements |

## What the numbers mean

- **Convergence** shows that a bus of 63 nodes reaches a deterministic schedule on its own, with no master and no configuration step.
- **Takeover** shows that a failed node's duties are picked up inside a bounded time, which is what a control loop needs to stay safe.
- **No replica disagreements** shows that every node, running the same function on the same table, always reached the same schedule.

## Next

A reference firmware implementation is underway in MPLAB X, and the WCET constants for the timing analysis are being derived from the compiled listing. Hardware measurements will be added here as they are taken.
