---
title: Module Requirements
---

## Sensor & Human–Machine Interface (HMI) Module Requirements

This section defines the functional and design requirements for the Sensor & HMI module of the R6 Recon Amphibot. The module is built around a PIC18F57K42 microcontroller and is responsible for acquiring motion and orientation data from a BNO055 IMU, presenting real-time telemetry on an SH1106 OLED display, accepting local operator input through debounced pushbuttons, and exchanging structured 64-byte UART packets with the adjacent nodes in the daisy chain.

Requirements were written before fabrication and are evaluated here against the final delivered hardware and firmware. Each row includes a minimum threshold below which the module would be considered non-functional, a target performance goal, whether the requirement is a stretch goal, and whether it was met in the V1 prototype.

---

## Module-Level Requirements

| Requirement           | Description                                                                               | Minimum Threshold                                      | Target                                                            | Stretch | Met in V1  |
| --------------------- | ----------------------------------------------------------------------------------------- | ------------------------------------------------------ | ----------------------------------------------------------------- | ------- | ---------- |
| PIC microcontroller   | Module shall use a PIC18F57K42 to manage all sensors, HMI outputs, and UART communication | MCU boots and executes firmware                        | Interrupt-driven firmware with stable main loop                   | No      | ✅ Yes     |
| Power regulation      | MCU and all peripherals shall operate from a regulated 3.3 V supply                       | Voltage within PIC18F57K42 operating range (2.3–3.6 V) | Stable 3.3 V rail with ≥ 100 mA headroom                          | No      | ✅ Yes     |
| Firmware architecture | Firmware shall use interrupt-driven design to avoid blocking delays                       | Main loop executes without stalls                      | All sensor reads and input events handled in ISRs or flags        | No      | ✅ Yes     |
| IMU sensor interface  | Module shall acquire orientation and motion data from a BNO055 over I²C                   | IMU detected and responding on bus                     | Stable quaternion/Euler output at ≥ 50 Hz                         | No      | ✅ Yes     |
| Software I²C          | Module shall implement bit-bang I²C to communicate with the BNO055 and OLED               | Transactions complete without bus lockup               | Clock stretching handled; 100 kHz effective rate                  | No      | ✅ Yes     |
| OLED display output   | Module shall display real-time system state and sensor data on an SH1106 OLED             | Legible text output rendered to display                | Text with formatted telemetry fields updated ≥ 5 Hz               | No      | ✅ Yes     |
| LED status indicators | Module shall provide LED-based visual cues for system state                               | At least one LED reflects a valid system state         | Distinct LED states for normal, fault, and UART activity          | No      | ✅ Yes     |
| Pushbutton input      | Module shall detect and respond to local operator input                                   | Button press reliably detected                         | Debounced, interrupt-driven input; no false triggers              | No      | ✅ Yes     |
| UART communication    | Module shall transmit and receive structured 64-byte packets on the daisy-chain bus       | Basic UART TX/RX functional at correct baud rate       | Correctly formatted packets passed upstream and downstream        | No      | ✅ Yes     |
| Hazard score output   | Module shall compute and output a scaled hazard indicator from IMU data                   | Binary safe/unsafe state available                     | 0–100 hazard score included in UART payload and displayed on OLED | Yes     | ⚠️ Partial |
| Demonstration clarity | Module shall produce observable cause-and-effect responses to operator input              | At least one visible response to a button press        | Simultaneous LED and OLED update on input event                   | Yes     | ✅ Yes     |

---

## Notes on V1 Outcomes

**Hazard score (partial):** A binary hazard flag was implemented and included in the UART packet. The scaled 0–100 score was computed in firmware but not consistently displayed on the OLED during the Innovation Showcase due to display refresh timing constraints. This is addressed in the V2 recommendations.

**Software I²C:** Hardware I²C on the PIC18F57K42 was non-functional during bring-up due to a peripheral conflict discovered post-fabrication. A software bit-bang I²C library was written and substituted with no impact on functionality, though at a lower effective clock rate than the hardware peripheral would have provided. Both the BNO055 and SH1106 communicated reliably at this rate throughout testing and the showcase.

**UART baud rate:** A baud rate mismatch between the PIC firmware and the ESP32 was identified and resolved during integration testing. Final confirmed baud rate used across all three boards was 9600 bps with 8N1 framing.

---

## Requirement Traceability

Each module requirement maps to one or more team-level system requirements defined on the [Team Project Requirements](https://egr314-s-2026-302.github.io/EGR314-Team302.github.io/03-Project-Requirements/Project-requirements/) page. The IMU interface requirement supports the system-level IMU data rate requirement. The UART requirement supports the daisy-chain packet routing requirement. Hazard score output supports the team-level hazard scoring stretch goal. All other module requirements are self-contained to the Sensor & HMI subsystem.
