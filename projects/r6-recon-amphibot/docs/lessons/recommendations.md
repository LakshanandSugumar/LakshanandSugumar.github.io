---
title: Recommendations for Future Students
---

# Recommendations for Future Students

**Lakshanand Sugumar — Team 302 — EGR314 Spring 2026**

---

1. **Read the full PIC18F57K42 datasheet section on your peripheral before touching MCC Melody.** MCC Melody generates initialization code automatically, but it does not validate that the clock source, pin remapping, and interrupt configuration are internally consistent. Understanding what the peripheral actually needs — from the datasheet, not from the tool — lets you catch configuration errors before they become post-fabrication debugging sessions.

2. **Do a bus scan as your first firmware test on every I²C device.** Write a simple loop that scans all 128 possible I²C addresses and prints which ones respond. Run this before writing any sensor driver code. It immediately tells you whether your wiring is correct, whether your pull-ups are working, and whether your I²C timing is producing valid transactions. This single test eliminates an entire category of bring-up failures.

3. **Scope your UART TX line before blaming firmware.** When UART communication fails between two boards, the instinct is to dive into the firmware. Scope the TX pin first. If the line is flat, the peripheral is not initialized or the pin is not remapped correctly. If the line is toggling but the receiver sees garbage, the baud rates do not match. If the line looks correct and the baud rates match, then look at the firmware. Scoping first saves hours.

4. **Keep a working software I²C implementation on hand.** Hardware peripherals can fail for reasons that are not your fault — layout issues, configuration bit errors, silicon errata. A portable software bit-bang I²C library lets you unblock the subsystem immediately while the root cause is investigated. This is not a workaround to be ashamed of — it is engineering judgment.

5. **Explicitly set every configuration bit that affects your peripherals.** MCC Melody leaves many configuration bits at their reset default values. For the PIC18F57K42, this includes LVP, MCLRE, and the oscillator selection bits. Some of these defaults are wrong for typical use — LVP enabled is the most dangerous because it silently breaks ICSP. Go through the configuration bits section of the datasheet for every bit that touches your peripherals and set it explicitly in your MCC project.

6. **Label every wire on your ribbon cable before integration.** When three boards are connected by ribbon cables during integration testing and something fails, unlabeled wires make fault isolation much harder. Spend ten minutes labeling VDD, GND, TX, and RX on both ends of every cable before the first system-level power-on. This is the cheapest debug tool available.

7. **Integrate boards one at a time, not all at once.** Bring up your own board fully in isolation first — confirm every peripheral works, every sensor responds, every LED lights. Then connect one neighboring board and verify communication in one direction. Then add the third board. Integrating all three boards simultaneously with untested firmware makes it nearly impossible to identify which board or which interface is causing a failure.

8. **The Innovation Showcase is a real deadline — start integration testing two weeks early.** The most stressful part of this project was not the firmware or the hardware — it was discovering integration issues the week of the showcase. Two weeks of integration buffer would have allowed the hazard score display issue to be resolved and the full feature set to be demonstrated. Build your schedule backward from the showcase date, not forward from when you think you will finish.
