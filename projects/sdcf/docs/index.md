---
title: "SDCF: Masterless Deterministic RS-485 Control Bus"
description: "SDCF (Self-Configuring Deterministic Control Fabric): PIC18F57Q83 nodes on a shared RS-485 bus self-allocate collision-free TDMA slots with no master and take over for failed peers within bounded frames."
hide:
  - navigation
  - toc
---

<div class="hero" markdown>
<p class="eyebrow">Independent research · Industrial networks · Aug 2024 – now</p>

# SDCF: A Bus With No Master

<p class="pitch">Self-Configuring Deterministic Control Fabric: microcontroller nodes on a shared RS-485 bus that give themselves collision-free time slots and cover for each other's failures, with no master node and no offline engineering.</p>
<p class="meta">Target: IEEE conference paper · Technical manuscript ~7,200 words</p>
<p class="tag-row"><span>RS-485</span><span>TDMA</span><span>Deterministic scheduling</span><span>PIC18F57Q83</span><span>Python</span><span>JavaScript</span><span>MPLAB X</span></p>

[How it works](how-it-works.md){ .md-button .md-button--primary } [Results](results.md){ .md-button } [:material-email-outline: Source available on request](mailto:lakshanandsugumar@gmail.com?subject=Source%20files%20request%3A%20SDCF){ .md-button }
</div>

<div class="lp-media" markdown>
<div class="sim-frame"><iframe src="sim/index.html" height="660" loading="lazy" title="Interactive simulation"></iframe></div>
<p class="cap">Interactive: watch nodes claim slots with no master, collide and back off, then fail a node and watch its buddy take over. A teaching animation of the protocol idea; the research numbers come from the Python simulator.</p>
</div>

<div class="stat-row">
  <div class="stat"><span class="num">63</span><span class="label">Nodes converge…</span></div>
  <div class="stat"><span class="num">≤ 88 frames</span><span class="label">…to a collision-free schedule, worst case</span></div>
  <div class="stat"><span class="num">3 frames</span><span class="label">Peer takeover after a node fails (bound: 4)</span></div>
  <div class="stat"><span class="num">0</span><span class="label">Slot collisions across 1,436 runs</span></div>
</div>

## The problem

Factory machines are coordinated by fieldbuses such as EtherCAT, PROFINET IRT or TSN. They are deterministic, but they depend on a master node or a schedule engineered offline. If the master fails, or a node is added, someone has to intervene.

## The idea

SDCF puts low-cost microcontrollers (PIC18F57Q83) on a shared RS-485 multi-drop bus. Every node runs the same schedule function on a replicated table, so all nodes reach the same answer independently:

- **Masterless slot self-allocation.** Nodes claim TDMA slots among themselves and converge to a collision-free schedule.
- **Join and admission.** New nodes join with contention and binary exponential backoff; admission control keeps the schedule within its period.
- **Buddy fault takeover.** When a node goes silent, a designated peer takes over its duties within a bounded number of frames.

The defensible gap: masterless, hard-deterministic slot self-allocation, with control-period-aware bandwidth assignment and bounded-latency peer fault takeover, on sub-$10 hardware over a wired multi-drop bus.

## Explore

<div class="grid cards" markdown>

-   :material-cog-transfer-outline: **[How it works](how-it-works.md)**

    Slots, anchor election, joining, admission control and buddy takeover.

-   :material-chart-timeline-variant: **[Results](results.md)**

    Convergence, takeover latency and collision-free operation in simulation.

-   :material-tools: **[Tooling](tooling.md)**

    Python simulator, bit-identical JavaScript port, browser visualizer and firmware.

</div>

!!! note "Source files"
    The code and design files for this project are shared on request. [Email me](mailto:lakshanandsugumar@gmail.com?subject=Source%20files%20request%3A%20SDCF) and I'll send them over.

<div class="lp-cta" markdown>
## Want to go deeper?

<p>Read the full write-up, browse the source, or see the rest of my work.</p>

[:material-email-outline: Source available on request](mailto:lakshanandsugumar@gmail.com?subject=Source%20files%20request%3A%20SDCF){ .md-button .md-button--primary } [All projects](https://lakshanandsugumar.github.io/projects/){ .md-button } [Portfolio](https://www.lakshanand.dev){ .md-button }
</div>
