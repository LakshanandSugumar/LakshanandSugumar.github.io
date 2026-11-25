<div class="hero" markdown>
<p class="eyebrow">EGR 456 · Spring 2026 · ASU Polytechnic</p>

# Vision-Guided 3-Axis Pick-and-Place

<p class="pitch">A three-axis robotic manipulator that finds objects with a camera and picks and places them on its own, with real-time motor control in embedded C on a PSoC 5LP.</p>

<p class="tag-row"><span>PSoC 5LP</span><span>Embedded C</span><span>OpenCV</span><span>PID</span><span>Motor control</span></p>

[Results](#results){ .md-button .md-button--primary } [System design](#system-design){ .md-button }
</div>
<div class="stat-row">
  <div class="stat"><span class="num">92.5%</span><span class="label">Success rate (74 / 80 trials)</span></div>
  <div class="stat"><span class="num">±2.5 mm</span><span class="label">Positioning accuracy</span></div>
  <div class="stat"><span class="num">±1.0 mm</span><span class="label">Repeatability</span></div>
  <div class="stat"><span class="num">4.8 s</span><span class="label">Mean cycle time</span></div>
</div>

## System design

```mermaid
flowchart LR
  CAM[Camera] --> CV[OpenCV object localization]
  CV --> PID[Closed-loop PID correction]
  PID -->|setpoints| PSOC[PSoC 5LP<br/>embedded C motor control]
  PSOC --> DRV[Motor drivers]
  DRV --> ARM[3-axis manipulator + gripper]
  ARM -. position feedback .-> PID
```

| Layer | What I did |
| --- | --- |
| Vision | Object localization in OpenCV to find each target in the workspace |
| Control | Closed-loop PID correction that compensates positioning error while grasping |
| Embedded | Real-time motor control written in embedded C on the PSoC 5LP |
| Hardware | Integrated motor drivers, sensors and control hardware; board-level bring-up and I/O validation |
| Verification | End-to-end system tests over the full workspace |

## Results

80 autonomous pick-and-place trials across a **350 × 200 × 150 mm** workspace:

| Metric | Result |
| --- | --- |
| Successful picks | **74 / 80 (92.5%)** |
| Positioning accuracy | ±2.5 mm |
| Repeatability | ±1.0 mm |
| Mean cycle time | 4.8 s per pick-and-place |

Repeatability is tighter than accuracy, which points to a systematic offset rather than random error: the arm lands in the same place each time, a little away from the target. That makes camera-to-arm calibration the clearest next improvement.

## Skills used

PSoC 5LP · PSoC Creator · embedded C · OpenCV · PID tuning · motor drivers · hardware bring-up · system verification

!!! note "Source code"
    The code and design files for this project are not public yet. This page documents what was built and measured.
