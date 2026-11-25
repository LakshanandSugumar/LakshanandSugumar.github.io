# Research paper ideas

Ranked by fit with the testbed and the time available (November to February). The
simulation results in `sim/figures/` are already preliminary evidence for #1 and #2.

## 1. Model-based fault detection and isolation for joint actuators (strongest; thesis-sized)

**Question.** Can a joint actuator tell a *sensor* fault from a *mechanical* event
using only signals the FOC loop already has?

**Idea.** Two residuals that respond to different things: the back-EMF voltage
residual reacts to encoder faults but not to load torque; the momentum observer
reacts to external torque but not to encoder faults (while the encoder is healthy).
Together they isolate the fault class, then the drive picks the right reaction.

**Evidence so far (sim).** A silent encoder freeze (no error flag) is caught in 3.3 ms;
a collision is caught 0.8 ms after contact with 0.12 Nm peak contact torque, against
30 ms and 0.45 Nm with a following-error check.

**Hardware experiments.** Encoder unplug / magnet shift / injected freeze, phase
open circuit, collisions at several speeds, payload changes. Report detection
latency, false-alarm rate over long normal runs (sine, chirp, random moves), and
ROC curves versus threshold.

**Venues.** IEEE/ASME AIM, IEEE IECON, IFAC SAFEPROCESS, IEEE Robotics and
Automation Letters (with a strong hardware section).

## 2. Encoder-fault-tolerant control: keep the joint alive instead of stopping

Extend #1: on ENC_MODEL, switch to a sensorless back-EMF observer (above a minimum
speed) or to a "limp home" torque mode, so a humanoid leg does not drop. Measure the
transition transient and how far the joint degrades. This is the natural thesis
chapter after #1 and maps directly to humanoid reliability.

## 3. Data-driven friction and cogging compensation from logged data

Use the 1 kHz logs to fit position-dependent cogging plus friction (lookup table, or
a small network), feed it forward, and report the tracking-error drop on top of the
model feedforward. Easy to execute, good for a workshop or student paper; it also
makes the main demo look better.

## 4. Cross-project: from joint-level faults to cell-level OEE (with the Siemens project)

Publish the actuator's fault events and health residuals over OPC UA (Python
`asyncua` server) into the production-cell project, treat the joint as one station
of the simulated cell, and quantify how detection latency and fault isolation
change availability and OEE. Frame it as a digital-twin / Industry 4.0 health
monitoring architecture. Fits IEEE ETFA, CASE or INDIN, which industrial
automation teams read.

## 5. Siemens cell alone (lower priority)

A virtual-commissioning workflow (Factory I/O + PLCSIM Advanced + S7-GRAPH +
OPC UA dashboard) as a teaching or rapid-prototyping case study, for example for
ASEE or an IFAC education workshop. Useful for the CV; less novel as research.

## Suggested path

Write #1 as the main paper (draft by early February, using sim + first hardware
results), keep #2 as the thesis extension, and do #4 only if the Siemens project
reaches OPC UA by mid-January. Ask an ASU faculty member in controls or robotics
early whether they would co-advise; a hardware FDI paper is easier to publish with
a faculty co-author.
