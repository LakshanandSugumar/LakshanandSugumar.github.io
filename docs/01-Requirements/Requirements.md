---
title: Module's Requirements
---

## Sensor & Human–Machine Interface (HMI) Module Requirements

This section defines the functional and design requirements for the Sensor & Human–Machine Interface (HMI) module of the Amphibot V1, based on the selected Concept 3 design. This module is responsible for gathering key sensor data, presenting clear and intuitive feedback to the operator, and supporting low-cognitive-load interaction during hazardous environment exploration.

In alignment with Concept 3’s emphasis on fast deployment, simple operation, and strong situational awareness, the requirements below focus on reliable sensor acquisition, clear visual and tactile user cues, and robust communication with other system modules. Each requirement includes a minimum acceptable performance level to avoid system failure, a target performance goal, and identification of stretch goals that enhance usability or system clarity beyond baseline functionality.

## Functional & Design Requirements

| Requirement             | Description                                                                                   | Minimum Threshold (Not Failure)           | Target Measurement                                             | Stretch |
| ----------------------- | --------------------------------------------------------------------------------------------- | ----------------------------------------- | -------------------------------------------------------------- | ------- |
| PIC microcontroller     | Module shall use a PIC microcontroller to manage sensors, HMI outputs, and UART communication | MCU boots and executes firmware reliably  | PIC MCU with sufficient GPIO, I²C, UART, and interrupt support | No      |
| Power regulation        | MCU and peripherals shall operate from a regulated low-voltage supply                         | Stable voltage within MCU operating range | Dedicated regulated 3.3 V rail for MCU and sensors             | No      |
| Firmware architecture   | System shall use an interrupt-driven design to maintain responsiveness                        | Main loop runs without blocking delays    | Fully interrupt-driven sensor sampling and input handling      | No      |
| IMU sensor interface    | Module shall acquire motion and orientation data from an IMU                                  | IMU detected and readable                 | Stable IMU data at ≥ 50 Hz                                     | No      |
| Hazard data abstraction | Module shall support output of a simplified hazard indicator based on sensor inputs           | Binary hazard state available             | Scaled hazard score output for display/telemetry               | Yes     |
| OLED display output     | Module shall present system and hazard information visually                                   | Text output visible and legible           | Text with simple graphics/icons                                | No      |
| LED status indicators   | Module shall provide immediate visual cues for system state                                   | At least one visible status LED           | Color-coded LEDs for mode/state indication                     | No      |
| Pushbutton input        | Module shall detect local user input                                                          | Button press reliably detected            | Debounced, interrupt-based input handling                      | No      |
| UART communication      | Module shall exchange data with other system modules                                          | Basic UART messaging functional           | Structured sensor and status messages                          | No      |
| Demonstration clarity   | Module shall visibly demonstrate cause-and-effect interaction                                 | At least one clear response to input      | Multiple simultaneous cues (LED + display)                     | Yes     |

---
