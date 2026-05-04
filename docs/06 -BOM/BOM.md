---
title: Bill of Materials
---

# Sensor & HMI Subsystem — Bill of Materials

**Lakshanand Sugumar — Team 302 — EGR314 Spring 2026**

---

## Overview

This Bill of Materials lists every electronic component on the Sensor & HMI PCB for Team 302. It covers the microcontroller, power regulation, sensors, display, passive components, user interface hardware, and programming interface. All components are documented with manufacturer part numbers, vendor sources, reference designators, and unit costs to support full traceability and PCB fabrication verification.

Spare quantities are included on passives and switches to cover assembly margin and potential rework.

---

## Bill of Materials

| Part Name / Description                         | Qty | Unit Cost | Total Cost | Manufacturer      | Manufacturer #         | Vendor              | Ref Des    |
| ----------------------------------------------- | --- | --------- | ---------- | ----------------- | ---------------------- | ------------------- | ---------- |
| PIC18F57K42 Microcontroller (QFN-48)            | 1   | $2.85     | $2.85      | Microchip         | PIC18F57K42-I/MR       | Digi-Key            | U1         |
| Buck Regulator 3.3 V 3 A (TSOT23-6)             | 2   | $0.71     | $1.42      | Diodes Inc.       | AP63203WU-7            | Digi-Key            | U2         |
| BNO055 9-DOF IMU (LGA-28)                       | 1   | $5.22     | $5.22      | Bosch             | BNO055                 | Digi-Key            | U3         |
| HDC2080 Temp/Humidity Sensor (WSON-6)           | 2   | $1.94     | $3.88      | Texas Instruments | HDC2080DMBR            | Digi-Key            | U4         |
| SH1106 1.3" 128×64 OLED Display (I²C)           | 1   | $6.50     | $6.50      | Various           | SH1106 module          | Amazon / AliExpress | DSP1       |
| Schottky Diode 40 V 3 A (SMC)                   | 2   | $0.70     | $1.40      | onsemi            | MBRS340T3G             | Digi-Key            | D1         |
| PTC Resettable Fuse 2.5 A                       | 1   | $4.60     | $4.60      | Littelfuse        | 015402.5DRT            | Digi-Key            | F1         |
| Fixed Inductor 6.8 µH                           | 2   | $0.28     | $0.56      | Bourns            | PM1812-6R8J-RC         | Digi-Key            | L1         |
| 22 µF 25 V X5R Capacitor (1812)                 | 3   | $1.09     | $3.27      | TDK               | C4532X5R1E226M250KA    | Digi-Key            | C1, C2, C3 |
| 0.1 µF X5R Bypass Capacitor (1812)              | 10  | $0.77     | $7.70      | TDK               | C4532X5R2J104K230KA    | Digi-Key            | C4–C13     |
| 4.7 kΩ 1% Resistor — I²C Pull-ups (0805)        | 4   | $0.22     | $0.88      | KOA Speer         | WK73R2JTTE4701F        | Digi-Key            | R1, R2     |
| 10 kΩ 1% Resistor — Switch Pull-ups (0805)      | 4   | $0.22     | $0.88      | KOA Speer         | WK73R2JTTE1002F        | Digi-Key            | R3, R4     |
| 220 Ω 1% Resistor — LED Current Limiting (0805) | 25  | $0.07     | $1.75      | Yageo             | RC0805FR-07220RL       | Digi-Key            | R5, R6     |
| 470 Ω 1% Resistor — General Purpose (0805)      | 25  | $0.37     | $9.25      | Yageo             | RC0805FR-07470RL       | Digi-Key            | R7–R10     |
| Tactile Switch SPST-NO (SMD)                    | 10  | $0.13     | $1.31      | C&K               | PTS636SM43SMTR LFS     | Digi-Key            | SW1, SW2   |
| LED Red Diffused 0805                           | 3   | $0.12     | $0.36      | Rohm              | SML-D12U1WT86          | Digi-Key            | D2         |
| LED Green Diffused 0805                         | 3   | $0.10     | $0.30      | LITEON            | LTST-C170GKT           | Digi-Key            | D3         |
| 2×4 IDC Header — Ribbon Cable Connector         | 2   | $0.50     | $1.00      | Various           | 2×4 2.54mm IDC         | Digi-Key            | J1, J2     |
| 5-pin ICSP Header                               | 1   | $0.30     | $0.30      | Various           | 1×5 2.54mm male header | Digi-Key            | J3         |

---

## Vendor Cost Breakdown

### Digi-Key Order

| Item               | Cost       |
| ------------------ | ---------- |
| Parts subtotal     | $46.43     |
| Tax                | $4.71      |
| Shipping           | $6.99      |
| Tariff             | $0.52      |
| **Digi-Key Total** | **$58.65** |

### Display Order (Amazon / AliExpress)

| Item               | Cost                     |
| ------------------ | ------------------------ |
| SH1106 OLED module | $6.50                    |
| Shipping           | $0.00 (Prime / included) |
| **Display Total**  | **$6.50**                |

---

## Total Cost Summary

|                                                | Cost       |
| ---------------------------------------------- | ---------- |
| Parts subtotal (all vendors, pre-tax/shipping) | $52.93     |
| **Grand total (including tax and shipping)**   | **$65.15** |

---

## Notes

- **PIC18F57K42** replaces the originally planned PIC18F47Q10. The 57K42 was selected for its larger flash and RAM headroom.
- **BNO055** is listed as the bare IC (LGA-28), not the Adafruit breakout board. The bare IC was used on the fabricated PCB.
- **SH1106** is listed as a module rather than a bare driver IC, sourced separately and mounted as a sub-assembly on the PCB.
- **I²C pull-up resistors (4.7 kΩ)** serve the shared software I²C bus used by the BNO055, SH1106, and HDC2080.
- **Switch pull-up resistors (10 kΩ)** replaced the 4.7 kΩ values from the original design to reduce static current draw from the pull-up network.
- **Spare quantities** on passives, switches, and LEDs account for assembly margin and potential rework during bring-up.
- The AP63203WU-7 is rated at **3 A**, not 2 A as previously documented.
- Reference designators are assigned based on the final fabricated schematic.

---

## Downloads

- [BOM (Excel)](BOM.xlsx)
- [BOM (PDF)](BOM.pdf)
