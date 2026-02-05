---
title: Module's Block Diagram
tags:
  - Sensor
  - HMI
  - Power
  - Communication
---

## Overview

This block diagram represents the **Sensor & Human–Machine Interface (HMI) Module** for **Amphibot V1**, based on the selected **Concept 3** system architecture. The module is centered around a **Microchip PIC18F47Q10 microcontroller** and is responsible for sensor data acquisition, operator interaction, system state indication, and communication with other robot subsystems.

In alignment with Concept 3’s goals of **fast deployment, simple operation, and strong situational awareness**, the diagram illustrates how regulated power, sensing, user inputs, and visual outputs are integrated to provide reliable, low-cognitive-load interaction in hazardous environments.

The block diagram highlights the following key aspects:

- **Power Levels:** A 12 V external DC power source is regulated down to a 3.3 V rail, supplying the microcontroller and all connected peripherals.
- **Sensors:** An IMU connected via the I²C bus provides motion and orientation data for hazard awareness and system feedback.
- **Human–Machine Interface:** An OLED display, pushbuttons, and a status LED provide clear visual and tactile feedback to the operator.
- **Communication:** UART enables structured data exchange with other Amphibot modules, while ICSP supports firmware programming and debugging.
- **Team Connections:** Upstream and downstream headers expose power and communication interfaces for integration with other team-designed subsystems.

Overall, this block diagram provides a high-level functional view of how the Sensor & HMI Module satisfies the defined product requirements while maintaining modularity and clear signal flow.

## Example Block Diagram

![Individual Block Diagram](laksh_blockdiagram.drawio)
