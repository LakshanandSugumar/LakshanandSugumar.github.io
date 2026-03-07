---
title: Welcome
tags:
  - EGR314
  - Amphibot
  - SensorSubsystem
---

<center>
<font size= "6">Lakshanand's Datasheet</font><br>
as part of<br>
<font size= "8"> Amphibot V1</font><br>
for<br>
<font size= "5"> Team 302 </font><br>

**Last Updated: March 6, 2026**

</center>

---

## Introduction

This datasheet presents the design and implementation of the **Sensor + Human–Machine Interface (HMI) subsystem** for Amphibot V1 (EGR 314 – Spring 2026).

It provides a standalone overview of my subsystem’s purpose, hardware design, firmware architecture, and role within the team’s UART daisy-chained robot platform.

---

## Project Summary

Amphibot V1 is a three-PCB modular reconnaissance robot inspired by R6 drone mechanics. The system includes:

- PIC 18F47K42 microcontroller
- Sensor + HMI Board (PIC-based)
- Actuator Control Board

The boards communicate using a standardized 64-byte UART packet structure in a daisy-chain configuration.

The Sensor + HMI subsystem:

- Collects IMU and environmental data
- Computes a simplified hazard score
- Displays system status on an SPI OLED
- Handles local button input and emergency stop
- Forwards structured UART messages between boards

For the complete team documentation, click [Team Report](https://egr314-s-2026-302.github.io/EGR314-Team302.github.io/)

---

## My Contribution

I designed and implemented the complete **Sensor + HMI subsystem**, including:

- Component selection and PCB design
- PIC-based firmware (interrupt-driven)
- Sensor integration (IMU + temp/humidity)
- OLED display interface
- UART packet parsing and forwarding
- Emergency stop handling

---

You can navigate to the main sections of our report using the links below:

- [Requirements](https://lakshanandsugumar.github.io/01-Requirements/Requirements/)
- [Block Diagram](https://lakshanandsugumar.github.io/02-Block-Diagram/Block-Diagram/)
- [Component Selection](https://lakshanandsugumar.github.io/03-Component-Selection/Component-Selection/)
- [Microcontroller Selection](https://lakshanandsugumar.github.io/04-%20Microcontroller%20Selection/mcs/)
- [Power Budget](https://lakshanandsugumar.github.io/05-%20Power%20Budget/power/)
- [Bill Of Material](https://lakshanandsugumar.github.io/06%20-BOM/BOM/)
- [Schematics](https://lakshanandsugumar.github.io/07%20-Schematic/schematic/)
- [PCB](https://lakshanandsugumar.github.io/08%20-PCB/pcb/)

---

## Downloads

- 📦 [Download Cadence Project](EGR314_Design_Review.zip)
- 📦 [Download Cadence Symbols](symbols_design_review.zip)
