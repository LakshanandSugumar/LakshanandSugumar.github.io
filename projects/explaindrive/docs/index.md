---
hide:
  - navigation
  - toc
---

<div class="hero" markdown>
<p class="eyebrow">Independent project · Autonomous driving · Nov 2026</p>

# ExplainDrive: An Explainable Reasoning Planner

<p class="pitch">A small, fully inspectable driving stack that reasons about a scene, plans a path, says why in plain English, and checks that those explanations are true.</p>
<p class="meta">Pure Python (NumPy, Matplotlib). No GPU, no CARLA, no datasets. Every result regenerates with one command.</p>
<p class="tag-row"><span>Motion planning</span><span>Frenet planner</span><span>Explainable AI</span><span>Closed-loop sim</span><span>Python</span></p>

[Scenarios & results](results.md){ .md-button .md-button--primary } [Design notes](design.md){ .md-button } [:fontawesome-brands-github: View source files](https://github.com/LakshanandSugumar/explainable-av-planner){ .md-button }
</div>

<div class="lp-media" markdown>
![The car slows for a crosswalk it cannot see, before anyone appears.](img/occluded_crosswalk.gif)
<p class="cap">The car slows for a crosswalk it cannot see, before anyone appears.</p>
</div>

<div class="stat-row">
  <div class="stat"><span class="num">6</span><span class="label">Hard closed-loop scenarios</span></div>
  <div class="stat"><span class="num">63%</span><span class="label">Fewer reasoner calls with adaptive reasoning</span></div>
  <div class="stat"><span class="num">500 ms</span><span class="label">Latency the stack tolerates collision-free</span></div>
  <div class="stat"><span class="num">100%</span><span class="label">Explanation necessity & completeness</span></div>
</div>


> *"My view of the crosswalk 11 m ahead is blocked by T1; limiting speed to 5.3 m/s so I can stop if someone steps out."* The car slows **before** it can see anyone. When the pedestrian appears from behind the truck, it stops comfortably. The reactive baseline in the same scene hits the pedestrian.

## The idea

Recent "reasoning" driving models plan a path *and* explain their decisions. ExplainDrive builds the same **reason → plan → explain** loop from first principles, small enough for one engineer to understand, test and measure end to end:

- a **scene reasoner** that turns what the car sees into driving constraints *and* the reasons for them,
- a **Frenet sampling trajectory planner** that drives inside those constraints,
- a **closed-loop simulator** with occlusion-aware perception, a kinematic bicycle model and 6 hard scenarios,
- and four experiments a reasoning model should face: baseline comparison, adaptive (fast/slow) reasoning, latency sensitivity for edge hardware, and explanation faithfulness checked with counterfactuals.

```mermaid
flowchart LR
  P[Perception<br/>occlusion-aware] --> L[Latency buffer]
  L --> R[Scene reasoner<br/>constraints + rationale]
  R --> PL[Frenet sampling planner]
  PL --> C[Pure pursuit + speed control]
  C --> W[World: kinematic bicycle]
  W --> P
  R --> X[Plain-English explanation]
  X --> F[Faithfulness check<br/>counterfactual deletion]
```

## Explore

<div class="grid cards" markdown>

-   :material-car-emergency: **[Scenarios & results](results.md)**

    Six scenarios with animations, and the four experiments with full tables.

-   :material-cog-outline: **[Design notes](design.md)**

    Loop timing, the constraint-based reasoner, the planner, the explanations and how faithfulness is tested.

</div>

<div class="lp-cta" markdown>
## Want to go deeper?

<p>Read the full write-up, browse the source, or see the rest of my work.</p>

[:fontawesome-brands-github: View source files](https://github.com/LakshanandSugumar/explainable-av-planner){ .md-button .md-button--primary } [All projects](https://lakshanandsugumar.github.io/projects/){ .md-button } [Portfolio](https://www.lakshanand.dev){ .md-button }
</div>
