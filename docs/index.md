---
title: Welcome
tags:
  - EGR314
  - R6ReconAmphibot
  - SensorSubsystem
---

<center>
<font size="6">Lakshanand Sugumar — Individual Datasheet</font><br>
as part of<br>
<font size="8">R6 Recon Amphibot</font><br>
for<br>
<font size="5">Team 302 — EGR314 Spring 2026</font><br>

**Last Updated: May 2026**

</center>

---

## Introduction

This datasheet presents the design and implementation of the **Sensor + Human–Machine Interface (HMI) subsystem** for the R6 Recon Amphibot (EGR314 – Spring 2026).

It provides a standalone technical record of my subsystem's hardware design, firmware architecture, component selection, and role within the team's three-node shared UART bus platform.

---

## Project Summary

The R6 Recon Amphibot is a three-board modular reconnaissance robot. The three subsystems are:

- **Sensor + HMI Board** — Lakshanand (this datasheet) — PIC18F57Q83
- **Wireless Gateway Board** — Mihir — ESP32 with WiFi, MQTT, and camera
- **Actuator Control Board** — Raunak — PIC18F with SPI motor driver

All three boards communicate over a shared UART bus at 9600 baud using AZ/YB framed ASCII packets.

The Sensor + HMI subsystem:

- Acquires orientation data from a BNO055 9-DOF IMU over software I²C
- Reads temperature and humidity from an HDC2080 sensor
- Displays real-time telemetry across four pages on an SH1106 OLED
- Handles local pushbutton input and page switching
- Publishes sensor data to the wireless gateway every 5 seconds
- Relays motor commands from the gateway to the actuator board
- Acts on emergency stop broadcasts immediately

For complete team documentation: [Team 302 Project Report](https://egr314-s-2026-302.github.io/EGR314-Team302.github.io/)

---

## My Contribution

I designed and implemented the complete Sensor + HMI subsystem, including:

- Component selection for all ICs, passives, and connectors
- PCB schematic capture and layout in KiCad
- Software bit-bang I²C library for BNO055, SH1106, and HDC2080
- BNO055 NDOF fusion mode driver and Euler angle parsing
- SH1106 OLED framebuffer driver with four switchable display pages
- HDC2080 temperature and humidity driver
- UART packet construction, parsing, routing, and relay firmware
- Emergency stop handling and ACK response
- Power budget analysis and regulator selection

---

## Navigation

- [Requirements](https://lakshanandsugumar.github.io/01-Requirements/Requirements/)
- [Block Diagram](https://lakshanandsugumar.github.io/02-Block-Diagram/Block-Diagram/)
- [Component Selection](https://lakshanandsugumar.github.io/03-Component-Selection/Component-Selection/)
- [Schematic & Pin Allocation](https://lakshanandsugumar.github.io/04-Microcontroller-Selection/mcs/)
- [Power Budget](https://lakshanandsugumar.github.io/05-Power-Budget/power/)
- [Bill of Materials](https://lakshanandsugumar.github.io/06-BOM/BOM/)
- [Schematic](https://lakshanandsugumar.github.io/07-Schematic/schematic/)
- [PCB](https://lakshanandsugumar.github.io/08-PCB/pcb/)
- [API & UART Protocol](https://lakshanandsugumar.github.io/09-API/api/)
- [Hardware Version 2.0](https://lakshanandsugumar.github.io/10-Hardware-V2/hardwarev2/)
- [Resources](https://lakshanandsugumar.github.io/11-Resources/resources/)
- [Lessons Learned](https://lakshanandsugumar.github.io/12-Lessons-Learned/lessons/)
- [Recommendations](https://lakshanandsugumar.github.io/13-Recommendations/recommendations/)
- [Reflection](https://lakshanandsugumar.github.io/14-Reflection/reflection/)

---

## Downloads

- [Cadence Project (.zip)](./07-Schematic/EGR314_Design_Review.zip)
- [Cadence Symbols (.zip)](./07-Schematic/symbols_design_review.zip)
- [Team API Reference (.docx)](./09-API/EGR314_Team302_API_Reference.docx)
