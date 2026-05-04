---
title: Schematic & Pin Allocation
---

# PIC18F57K42 — Sensor & HMI Subsystem

---

## 1. Microcontroller Overview

**Selected:** Microchip PIC18F57K42 (QFN-48)

The PIC18F57K42 was selected for its large flash (128 KB), ample RAM (8 KB), and full peripheral set. A hardware I²C conflict was discovered post-fabrication; all I²C communication to the BNO055, SH1106, and HDC2080 is handled via a software bit-bang implementation on general-purpose GPIO pins.

| Parameter         | Value                 |
| ----------------- | --------------------- |
| Architecture      | 8-bit PIC18           |
| Flash             | 128 KB                |
| RAM               | 8 KB                  |
| Operating Voltage | 2.3–3.6 V             |
| Package           | QFN-48                |
| Toolchain         | MPLAB X + MCC Melody  |
| Programmer        | Microchip SNAP (ICSP) |

---

## 2. Pin Layout

![PIC18F57K42 Pin Layout](pinlayout.png)

---

## 3. Pin Allocation

![Pin Allocation](pinallocate1.png)

### Full Pin Assignment Table

| Pin            | Port  | Function           | Connected To                        |
| -------------- | ----- | ------------------ | ----------------------------------- |
| SCL (bit-bang) | RC3   | Software I²C Clock | BNO055 SCL, SH1106 SCL, HDC2080 SCL |
| SDA (bit-bang) | RC4   | Software I²C Data  | BNO055 SDA, SH1106 SDA, HDC2080 SDA |
| TX             | RC6   | UART Transmit      | Downstream daisy-chain RX           |
| RX             | RC7   | UART Receive       | Upstream daisy-chain TX             |
| SW1            | RB0   | Digital Input      | Tactile switch 1                    |
| SW2            | RB1   | Digital Input      | Tactile switch 2                    |
| LED1           | RA2   | Digital Output     | Status LED 1                        |
| LED2           | RA3   | Digital Output     | Status LED 2                        |
| PGC            | RB6   | ICSP Clock         | SNAP programmer                     |
| PGD            | RB7   | ICSP Data          | SNAP programmer                     |
| MCLR           | Pin 1 | Reset              | SNAP programmer MCLR                |
| VDD            | —     | 3.3 V supply       | AP63203WU-7 regulator output        |
| VSS            | —     | Ground             | System GND                          |

> **Note on software I²C pins:** RC3 and RC4 were chosen because they map to the hardware MSSP peripheral footprint, keeping routing clean on the PCB even though the peripheral itself was not used.

---

## 4. System Architecture

The subsystem contains the following components, all operating at 3.3 V:

1. AP63203WU-7 — 3.3 V / 3 A buck regulator
2. PIC18F57K42 — main microcontroller
3. BNO055 — 9-DOF IMU (software I²C, address 0x28)
4. SH1106 — 1.3" 128×64 OLED display (software I²C, address 0x3C)
5. HDC2080 — temperature and humidity sensor (software I²C, address 0x40)
6. 2× tactile switches — debounced in firmware
7. 2× status LEDs — current-limited via 220 Ω series resistors
8. MPLAB SNAP ICSP header

---

## 5. Power Architecture

### Input

- 3.3 V received from upstream ribbon cable connector (regulated by ESP32 board)
- AP63203WU-7 on board provides local regulation if powered from a raw supply

### Decoupling

- 22 µF bulk capacitor at regulator output
- 0.1 µF ceramic at each VDD pin of PIC18F57K42, BNO055, SH1106, and HDC2080

### Power Domain

All logic and peripherals share a single 3.3 V rail. No separate analog or I/O domains.

---

## 6. Peripheral Configuration

### Software I²C Bus (Shared — BNO055 + SH1106 + HDC2080)

| Signal | MCU Pin | Pull-up         |
| ------ | ------- | --------------- |
| SCL    | RC3     | 4.7 kΩ to 3.3 V |
| SDA    | RC4     | 4.7 kΩ to 3.3 V |

**Device addresses on shared bus:**

| Device  | I²C Address | Address Pin Config        |
| ------- | ----------- | ------------------------- |
| BNO055  | 0x28        | ADR, PS0, PS1 tied to GND |
| SH1106  | 0x3C        | Fixed (module default)    |
| HDC2080 | 0x40        | ADDR pin tied to GND      |

No address conflicts. Bus arbitration is handled in firmware by sequencing transactions — BNO055 read → HDC2080 read → SH1106 framebuffer write.

### UART (Daisy-Chain)

| Signal | MCU Pin | Connection                     |
| ------ | ------- | ------------------------------ |
| TX     | RC6     | Downstream board RX (Actuator) |
| RX     | RC7     | Upstream board TX (ESP32)      |

- Baud rate: 9600 bps, 8N1
- Packet size: 64 bytes, team-defined structure
- Hardware UART peripheral (EUSART) — no conflict, functioned correctly

### Pushbuttons (×2)

| Switch | MCU Pin | Pull-up         | Logic      |
| ------ | ------- | --------------- | ---------- |
| SW1    | RB0     | 4.7 kΩ to 3.3 V | Active-low |
| SW2    | RB1     | 4.7 kΩ to 3.3 V | Active-low |

Both switches are debounced in firmware via flag-based interrupt handling. No floating inputs.

### Status LEDs (×2)

| LED  | MCU Pin | Series Resistor |
| ---- | ------- | --------------- |
| LED1 | RA2     | 220 Ω           |
| LED2 | RA3     | 220 Ω           |

LED states reflect: normal operation, fault/hazard condition.

### ICSP — MPLAB SNAP

| SNAP Signal | MCU Pin      |
| ----------- | ------------ |
| PGC         | RB6          |
| PGD         | RB7          |
| MCLR        | MCLR (Pin 1) |
| VDD         | 3.3 V        |
| GND         | GND          |

LVP (Low Voltage Programming) is **disabled** in configuration bits. Standard ICSP voltage levels apply. This was confirmed necessary after initial bring-up failures caused by incorrect LVP configuration.

---

## 7. Peripheral Usage Summary

| Peripheral          | Status      | Purpose                              |
| ------------------- | ----------- | ------------------------------------ |
| Software I²C        | ✅ Active   | BNO055, SH1106, HDC2080              |
| Hardware EUSART     | ✅ Active   | UART daisy-chain (TX/RX)             |
| GPIO — Input        | ✅ Active   | 2× pushbuttons                       |
| GPIO — Output       | ✅ Active   | 2× status LEDs                       |
| Hardware I²C (MSSP) | ⚠️ Not used | Peripheral conflict post-fabrication |
| SPI                 | ❌ Not used | OLED moved to I²C                    |
| ADC                 | ❌ Not used | No analog sensors in V1              |
| USB                 | ❌ Not used | Not required                         |

---

## 8. Electrical Validation

- No floating GPIO — all inputs have explicit pull-ups
- 4.7 kΩ pull-ups on shared I²C bus (single pair, shared across all three devices)
- 0.1 µF decoupling at every IC VDD pin
- 22 µF bulk capacitor at regulator output
- UART TX/RX not shorted — confirmed via continuity check during bring-up
- All devices confirmed 3.3 V compatible
- ICSP header wired and verified against SNAP pinout before first programming attempt
