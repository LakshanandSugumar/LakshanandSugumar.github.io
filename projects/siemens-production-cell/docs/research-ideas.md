# Research paper ideas

The production cell alone is a portfolio piece. Combined with the robot-joint actuator testbed (FOC on STM32, CAN/EtherCAT, Python logging) there are three realistic papers. Ranked by strength.

## 1. Actuator health data in the production cell's OEE loop (uses both projects)

**Question.** Can health indicators from a robot-joint actuator (phase current residuals, winding temperature, tracking error growth) predict a fault early enough that scheduling maintenance raises cell availability, compared with run-to-failure?

**Method.** The FOC testbed publishes health indicators over OPC UA (or CAN to a gateway). The cell's PLC or the Python side treats the actuator as a station resource. Inject degradations on the testbed (added friction, encoder noise, phase resistance change), feed the measured indicator traces into the simulated cell (`cellsim` already models faults, MTTR and OEE), and compare availability under run-to-failure, fixed-interval and condition-based maintenance.

**Why it is timely.** Humanoid robots built from exactly these actuators are moving into factory cells, and the integration of actuator-level health into line-level OEE is barely written about.

**Venues.** IEEE CASE, IEEE ETFA, IFAC conferences (check 2027 deadlines).

## 2. Current-signature fault detection on a robot-joint actuator

**Question.** How early and how reliably can a model-based residual (from the motor model already used for FOC tuning) detect common actuator faults, compared with a small learned detector, under realistic load profiles?

**Method.** Use the Simulink/Python motor model from the actuator project's first half to generate residuals; validate on hardware with injected faults. Report detection delay vs false-alarm rate (ROC), and compute cost on the STM32.

**Venues.** ACC, IEEE/ASME AIM, IECON. This is the natural seed of a master's thesis.

## 3. Data-driven supervision limits for PLC step sequences

**Question.** S7-GRAPH supervision times are usually set by hand (e.g. "2 s"). Can logged step durations set them statistically, trading false alarms against detection delay, and how much downtime does that save?

**Method.** Log step times per GRAPH step (from the cell over OPC UA, or from `cellsim`), fit a per-step distribution, set the limit at a target false-alarm rate, and compare detection delay and spurious stops with hand-set limits under injected jams. The simulator in this repo can already produce the data; the real cell gives validation.

**Venues.** IEEE ETFA work-in-progress track, INDIN, or a workshop. Small and fast, a good first paper from the Siemens project alone.

## Honest scope note

Simulation-only results usually land as conference or work-in-progress papers. Ideas 1 and 2 with hardware data from the actuator testbed are journal-capable. Idea 1 needs the testbed working by about February to be realistic for a mid-2027 submission.
