---
title: "IR Motion-Sensing Door Actuator (EGR 304)"
description: "EGR 304 home door automation: infrared distance-sensing subsystem on a PIC18F57Q43, signal conditioning, schematic and integration with motor, encoder and flex-sensor subsystems."
hide:
  - navigation
  - toc
---

<div class="hero" markdown>
<p class="eyebrow">EGR 304 · Fall 2025 · Team 103 · ASU Polytechnic</p>

# IR Motion-Sensing Door Actuator

<p class="pitch">A self-calibrating automatic door for private homes that opens when someone approaches and stops if anything is in its path. I designed the infrared distance-sensing subsystem from concept to working prototype.</p>
<p class="meta">Team 103: Isaac Smith, Christo Joseph, Seth Merwin, Lakshanand Sugumar · Instructor: Dr. Daniel M. Aukes</p>
<p class="tag-row"><span>PIC18F57Q43</span><span>IR sensing</span><span>Signal conditioning</span><span>PCB bring-up</span><span>Embedded C</span></p>

[My subsystem](subsystem.md){ .md-button .md-button--primary } [Team site](https://egr304-2025-f-103.github.io/){ .md-button } [:fontawesome-brands-github: View source files](https://github.com/EGR304-2025-F-103/egr304-2025-f-103.github.io){ .md-button }
</div>

<div class="lp-media" markdown>
![All four subsystems connected at the end of the course.](img/EGR304ProjectPhoto.png)
<p class="cap">All four subsystems connected at the end of the course.</p>
</div>

<div class="stat-row">
  <div class="stat"><span class="num">100%</span><span class="label">Design review score: functionality & documentation</span></div>
  <div class="stat"><span class="num">4</span><span class="label">Embedded subsystems in the product</span></div>
  <div class="stat"><span class="num">< $300</span><span class="label">Target consumer cost</span></div>
  <div class="stat"><span class="num">1</span><span class="label">Button press to self-calibrate</span></div>
</div>


## The product

Automatic doors are common in stores and offices, but rare in homes. Team 103 set out to bring a light-duty, inexpensive and easy-to-install automatic opener to any interior swinging door, aimed first at people with disabilities and their caregivers, and at anyone with their hands full.

After a simple install, one button press opens and closes the door while the sensors learn its normal path. From then on, the door opens when someone approaches from either side, waits, and closes, and it stops if an obstacle or an unusual change in its path appears. Four embedded subsystems, each designed by one team member, make that work:

```mermaid
flowchart LR
  IR[Infrared distance sensing<br/>Lakshanand] --> MOT[Motor subsystem]
  ENC[Rotary encoder] --> MOT
  FLEX[Resistive flex sensor] --> MOT
  MOT -->|drives| DOOR[Door]
```

The subsystems connect through 8-wire ribbon cables; the motor board acts as the hub and bridges the flex sensor and encoder during calibration.

## Demo video

The final integrated system: all four subsystems on breadboards, sharing one power source and communicating with each other as designed. (Per the team's notes, class-wide setbacks kept the team from soldering the final PCBs or building a housing and model door before the deadline.)

<div style="position:relative;padding-bottom:56.25%;height:0;overflow:hidden;border-radius:8px">
<iframe src="https://www.youtube-nocookie.com/embed/ntUvapRTiwo" title="EGR 304 Team 103 door automation demo" style="position:absolute;top:0;left:0;width:100%;height:100%;border:0" allow="accelerometer; encrypted-media; gyroscope; picture-in-picture" allowfullscreen></iframe>
</div>

## Explore

<div class="grid cards" markdown>

-   :material-motion-sensor: **[My subsystem](subsystem.md)**

    The infrared sensing front end, conditioning circuit, PCB and firmware.

-   :material-account-group-outline: **[User needs](team/user-needs.md)**

    Who the door is for and how existing products compare.

-   :material-clipboard-list-outline: **[Requirements](team/requirements.md)**

    Objective, stakeholders, user stories and requirements.

-   :material-sitemap: **[Block diagram](team/block-diagram.md)**

    How the four subsystems connect.

</div>

The team-project pages on this site are the Team 103 documentation, reproduced with credit to the whole team. The original is at [https://egr304-2025-f-103.github.io/](https://egr304-2025-f-103.github.io/).

<div class="lp-cta" markdown>
## Want to go deeper?

<p>Read the full write-up, browse the source, or see the rest of my work.</p>

[:fontawesome-brands-github: View source files](https://github.com/EGR304-2025-F-103/egr304-2025-f-103.github.io){ .md-button .md-button--primary } [All projects](https://lakshanandsugumar.github.io/projects/){ .md-button } [Portfolio](https://www.lakshanand.dev){ .md-button }
</div>
