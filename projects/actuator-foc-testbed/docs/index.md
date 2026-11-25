---
title: "STM32 FOC Robot-Joint Actuator with Fault Detection"
description: "Robot-joint actuator testbed: field-oriented control (FOC) on an STM32G474, AS5047P encoder, cascaded current, velocity and position loops, model-based fault detection (back-EMF residual, momentum observer) and a CAN + Python test stack. Simulation-first."
hide:
  - navigation
  - toc
---

<div class="hero" markdown>
<p class="eyebrow">Independent project · Robot actuators · Nov 2026 – now</p>

# Robot-Joint Actuator FOC Testbed

<p class="pitch">One robot joint built the way humanoid and legged-robot actuators are built: a brushless motor, a magnetic encoder, field-oriented control on an STM32G4, model-based fault detection and a CAN link to a Python test stack.</p>
<p class="meta">Simulation-first: every loop is designed and tuned in a plant model, implemented in hardware-independent C, and tested against the model before it drives a motor.</p>
<p class="tag-row"><span>STM32G474</span><span>FOC</span><span>C99</span><span>CAN</span><span>AS5047P</span><span>Python</span><span>SIL testing</span></p>

[See the results](results.md){ .md-button .md-button--primary } [How it works](design.md){ .md-button } [:fontawesome-brands-github: View source files](https://github.com/LakshanandSugumar/actuator-foc-testbed){ .md-button }
</div>

<div class="lp-media" markdown>
![Simulation: the joint tracking a trajectory, rejecting a load and catching faults.](img/actuator_sim_demo.gif)
<p class="cap">Simulation: the joint tracking a trajectory, rejecting a load and catching faults.</p>
</div>

<div class="stat-row">
  <div class="stat"><span class="num">0.037°</span><span class="label">Sine-tracking RMS error with feedforward</span></div>
  <div class="stat"><span class="num">3.3 ms</span><span class="label">Silent encoder freeze caught</span></div>
  <div class="stat"><span class="num">0.8 ms</span><span class="label">Collision detected by momentum observer</span></div>
  <div class="stat"><span class="num">67 ms</span><span class="label">90° step settle, 0% overshoot</span></div>
</div>


## What it is

Humanoid and legged robots are built from compact joint actuators: a brushless motor, a reduction, an encoder and a driver running field-oriented control (FOC) tens of thousands of times a second. This testbed is one of those joints, built to study how well it can be controlled and how quickly it can tell when something goes wrong.

The stack has three layers that share one design:

| Layer | What it does |
| --- | --- |
| **Simulation** (`sim/`) | Surface-PMSM plant with sensor models, cascaded FOC controller, fault monitor and 8 experiments that regenerate every figure on this site |
| **Firmware** (`firmware/`) | Hardware-independent C99 core (FOC math, cascade, fault detectors, CAN protocol, AS5047P driver) plus STM32G474 HAL glue; unit tests and software-in-the-loop against the plant model |
| **Host** (`host/`) | python-can link with a 1 kHz logger, test profiles, encoder calibration, sim-vs-hardware comparison and a virtual joint that speaks the real protocol |

```mermaid
flowchart LR
  subgraph Joint
    M[BLDC / PMSM motor] --- E[AS5047P magnetic encoder]
    D[STM32G474 FOC driver] -->|PWM| M
    E -->|SPI angle| D
  end
  D <-->|CAN| H[Python host: logger, tests, calibration]
  S[Plant model + controller sim] -. same algorithm, SIL tests .-> D
```

## Highlights

<div class="grid cards" markdown>

-   :material-sine-wave: **[Results](results.md)**

    Current, velocity and position loops, tracking, fault detection and robustness, each with its figure.

-   :material-function-variant: **[Design & Theory](design.md)**

    Plant model, loop-tuning derivations, the current-limit-aware braking curve and why the fault detectors work.

-   :material-chip: **[Firmware](firmware.md)**

    Layout, CubeMX settings, bring-up order and CPU budget for the STM32G474.

-   :material-swap-horizontal: **[CAN Protocol](can-protocol.md)**

    Every message, field and fault bit between the joint and the host.

-   :material-rocket-launch-outline: **[Getting Started](getting-started.md)**

    Run everything without hardware, then wire up and bring up the bench.

-   :material-file-document-outline: **[Fault-isolation paper](https://lakshanandsugumar.github.io/projects/actuator-fault-isolation/)**

    The IEEE paper draft that grew out of this testbed's fault detectors.

</div>

## Status

| Part | State |
| --- | --- |
| Plant model, controller, fault monitor, 8 experiments | Done, regression-tested in CI |
| Firmware core (C99) with unit and SIL tests | Done |
| STM32G474 HAL glue and CubeMX settings | Written, waiting for bench bring-up |
| Host tools and virtual joint | Done |
| Hardware results | Next: added beside the simulation results as the bench comes up |

<div class="lp-cta" markdown>
## Want to go deeper?

<p>Read the full write-up, browse the source, or see the rest of my work.</p>

[:fontawesome-brands-github: View source files](https://github.com/LakshanandSugumar/actuator-foc-testbed){ .md-button .md-button--primary } [All projects](https://lakshanandsugumar.github.io/projects/){ .md-button } [Portfolio](https://www.lakshanand.dev){ .md-button }
</div>
