---
title: Block Diagram
---

![Individual Block Diagram](laksh_blockdiagram.drawio.png)

## Overview

This block diagram represents the **Sensor & HMI subsystem** for **Team 302 – R6 Recon Amphibot**, built around a **Microchip PIC18F57K42 microcontroller**. The subsystem acquires motion and orientation data from a **Bosch BNO055 IMU**, presents real-time telemetry on an **SH1106 OLED display**, accepts local operator input through two debounced pushbuttons, and exchanges structured 64-byte UART packets with the upstream ESP32 wireless gateway board and the downstream PIC actuator board.

All I²C communication — to both the BNO055 and the SH1106 — is handled via a **software bit-bang I²C implementation** due to a hardware I²C peripheral conflict discovered during bring-up on the fabricated PCB.

Key features shown:

- **Power:** 3.3 V regulated rail supplying the PIC, BNO055, OLED, and all digital I/O
- **Sensor:** BNO055 IMU over software I²C for orientation and motion data
- **HMI:** SH1106 OLED display over software I²C, two pushbuttons, and status LEDs
- **Communication:** UART TX/RX for daisy-chain packet routing between subsystems
- **Programming:** ICSP interface for firmware loading via Microchip SNAP programmer

---

## Block Diagram Description

### Microcontroller

- **Microcontroller:** Microchip PIC18F57K42
- **Peripherals used:**
  - Software bit-bang I²C — BNO055 IMU and SH1106 OLED display
  - Digital Inputs — two pushbuttons (debounced via firmware)
  - Digital Outputs — status LEDs
  - UART — inter-module daisy-chain communication (9600 baud, 8N1)
  - ICSP — in-circuit programming and debug via Microchip SNAP

### Power Architecture

- **Input:** 3.3 V supplied from the upstream ribbon cable connector (regulated by the ESP32 board's onboard LDO)
- **Distribution:** Single 3.3 V rail shared by the PIC18F57K42, BNO055, SH1106 OLED, and all digital peripherals
- **Bypass capacitors:** 0.1 µF ceramic at each IC power pin for high-frequency decoupling

### Sensors

- **BNO055 IMU**
  - Interface: Software I²C (SDA/SCL, 2 signal pins, bidirectional)
  - I²C address: 0x28 (ADR pin tied to GND)
  - Protocol mode pins: PS0, PS1 tied to GND (I²C mode selected)
  - Pull-up resistors: 4.7 kΩ on SDA and SCL lines
  - Function: Euler angles, quaternion output, and linear acceleration for hazard scoring and telemetry

### Human–Machine Interface

- **SH1106 OLED Display**
  - Interface: Software I²C (shared bus with BNO055)
  - Resolution: 128 × 64 pixels
  - Function: Displays real-time IMU telemetry, hazard indicator, and UART packet status; updated at ≥ 5 Hz

- **Pushbuttons (×2)**
  - Interface: Digital Input (1 signal pin each, active-low with internal pull-up)
  - Function: Mode selection and manual trigger events; debounced in firmware via flag-based ISR handling

- **Status LEDs**
  - Interface: Digital Output (current-limited via series resistor)
  - Function: Visual indication of system state — normal operation, fault condition, and UART activity

### Communication Interfaces

- **UART (Daisy-Chain Bus)**
  - Baud rate: 9600 bps, 8N1 framing
  - Packet size: 64 bytes, structured per team-defined message format
  - Upstream: Receives command and routing packets from ESP32 wireless board
  - Downstream: Forwards relevant packets and appends sensor payload for actuator board

- **ICSP (In-Circuit Serial Programming)**
  - Connector: 5-pin header (MCLR, PGD, PGC, VDD, GND)
  - Programmer: Microchip SNAP
  - LVP disabled in configuration bits to allow standard ICSP voltage levels

### External Interfaces

- **Upstream Ribbon Cable Connector**
  - Receives: 3.3 V power, UART RX from ESP32 TX
  - Sends: UART TX to ESP32 RX

- **Downstream Ribbon Cable Connector**
  - Sends: 3.3 V power pass-through, UART TX to actuator board RX
  - Receives: UART RX from actuator board TX

---

## Downloads

- [Block Diagram (PDF)](laksh_blockdiagram.drawio.pdf)
- [View Block Diagram Online](https://drive.google.com/file/d/1TroP7_NFbSjZxFFsCl5taaWzNd-KRFt1/view?usp=sharing)
