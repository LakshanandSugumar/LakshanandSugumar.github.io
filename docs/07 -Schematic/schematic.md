---
title: Schematic
---

# Sensor & HMI Subsystem — Schematic

**Lakshanand Sugumar — Team 302 — EGR314 Spring 2026**

---

## Overview

This schematic shows the complete design for the Sensor & HMI subsystem of the R6 Recon Amphibot. It includes the **PIC18F57K42 microcontroller**, **AP63203WU-7 3.3 V / 3 A buck regulator**, **SH1106 1.3" 128×64 OLED display (software I²C)**, **BNO055 9-DOF IMU (software I²C)**, **HDC2080 temperature and humidity sensor (software I²C)**, **status LEDs**, **debounced pushbutton switches**, and the **Microchip SNAP ICSP programming interface**.

All three I²C devices — BNO055, SH1106, and HDC2080 — share a single software bit-bang I²C bus on the PIC18F57K42. Hardware I²C (MSSP) was not used due to a peripheral conflict discovered post-fabrication. UART TX/RX connects this board to the upstream ESP32 board and the downstream actuator board as part of the three-node daisy chain.

The schematic covers power distribution, bypass decoupling at every IC supply pin, reset circuitry, I²C bus pull-ups, UART routing, pushbutton pull-up networks, LED current limiting, and ICSP header wiring.

---

## Schematic

**Figure 01: Sensor & HMI Subsystem Schematic — Sheet 1**

![Schematic Sheet 1](/docs/07%20-Schematic/mosfet_hbridge/mosfet_hbridge_page-0001.jpg)

![Schematic Sheet 2](/docs/07%20-Schematic/mosfet_hbridge/mosfet_hbridge_page-0002.jpg)

## ![Schematic Sheet 3](/docs/07%20-Schematic/mosfet_hbridge/mosfet_hbridge_page-0003.jpg)

## Key Design Decisions Visible in Schematic

- **Software I²C bus:** SDA and SCL routed to RC3 and RC4 as GPIO. 4.7 kΩ pull-ups to 3.3 V shared across BNO055, SH1106, and HDC2080. Each device has a unique address (0x28, 0x3C, 0x40).
- **BNO055 mode pins:** PS0, PS1, and ADR all tied to GND, selecting I²C mode and address 0x28.
- **UART daisy-chain:** RC6 (TX) routes to downstream actuator board RX; RC7 (RX) receives from upstream ESP32 TX. 9600 baud, 8N1.
- **ICSP header:** PGC → RB6, PGD → RB7, MCLR → Pin 1. LVP disabled in configuration bits.
- **Bypass capacitors:** 0.1 µF ceramic at every VDD pin on the PIC18F57K42, BNO055, SH1106, and HDC2080. 22 µF bulk capacitor at regulator output.
- **LED current limiting:** 220 Ω series resistors on LED1 (RA2) and LED2 (RA3).
- **Switch pull-ups:** 10 kΩ to 3.3 V on SW1 (RB0) and SW2 (RB1), active-low logic.

---

## Downloads

- [Cadence Project (.zip)](EGR314_Design_Review.zip)
- [Cadence Symbols (.zip)](symbols_design_review.zip)
- [Schematic (PDF)](/docs/08%20-PCB/mosfet_hbridge.pdf)
