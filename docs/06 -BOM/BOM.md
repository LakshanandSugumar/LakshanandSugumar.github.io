# Bill of Materials

## PIC Sensor & Control Subsystem

### Team 302 – EGR314 Spring 2026

---

## Overview

This Bill of Materials (BOM) lists every electronic component required for Lakshanand Sugumar’s PIC Sensor & Control Subsystem for Team 302 (EGR314 Spring 2026).

It includes power regulation components, protection circuitry, sensors, passive components, user interface hardware, and debugging components required for PCB fabrication and subsystem integration.

All components are documented with manufacturer information, part numbers, supplier details, and schematic reference designators to ensure full traceability and verification compliance.

Spare quantities are included where necessary to ensure reliability during assembly, soldering, and system integration testing.

---

# Bill of Materials

### Table 01: Items below represent the complete BOM associated with this subsystem.

---

## Bill of Materials — PIC Sensor & Control Subsystem (Team 302)

| Part Name / Description                 | Qty | Unit Cost | Total Cost | Manufacturer      | Manufacturer #      | Vendor   | Reference |
| --------------------------------------- | --- | --------- | ---------- | ----------------- | ------------------- | -------- | --------- |
| Buck Regulator 3.3V 2A (TSOT23-6)       | 2   | $0.71     | $1.42      | Diodes Inc.       | AP63203WU-7         | Digi-Key | U?        |
| Schottky Diode 40V 3A (SMC)             | 2   | $0.70     | $1.40      | onsemi            | MBRS340T3G          | Digi-Key | D7        |
| PTC Resettable Fuse 2.5A                | 1   | $4.60     | $4.60      | Littelfuse        | 015402.5DRT         | Digi-Key | F1        |
| Fixed Inductor 6.8µH                    | 2   | $0.28     | $0.56      | Bourns            | PM1812-6R8J-RC      | Digi-Key | L1        |
| 22µF 25V X5R Capacitor                  | 3   | $1.09     | $3.27      | TDK               | C4532X5R1E226M250KA | Digi-Key | C4, C5    |
| 0.1µF X5R Capacitor                     | 10  | $0.77     | $7.70      | TDK               | C4532X5R2J104K230KA | Digi-Key | C15, C16  |
| 10kΩ 1% Resistor                        | 4   | $0.22     | $2.00      | KOA Speer         | WK73R2JTTE1002F     | Digi-Key | R?        |
| 220Ω 1% Resistor                        | 25  | $0.07     | $1.75      | Yageo             | RC1218FK-07220RL    | Digi-Key | R?        |
| 470Ω 1% Resistor                        | 25  | $0.37     | $1.85      | Yageo             | RC1218FK-07470RL    | Digi-Key | R?        |
| Tactile Switch SPST-NO                  | 10  | $0.13     | $1.31      | C&K               | PTS636SM43SMTR LFS  | Digi-Key | SW1–SW10  |
| LED Red Diffused 0805                   | 3   | $0.12     | $0.36      | Rohm              | SML-D12U1WT86       | Digi-Key | D5, D6    |
| LED Green Diffused 0805                 | 3   | $0.10     | $0.30      | LITEON            | LTST-C170GKT        | Digi-Key | D2, D4    |
| HDC2080 Humidity Sensor                 | 2   | $1.94     | $3.88      | Texas Instruments | HDC2080DMBR         | Digi-Key | U5        |
| 9-DOF Absolute Orientation IMU (BNO055) | 1   | $34.95    | $34.95     | Adafruit          | PID: 2472           | Adafruit | U18       |

---

# Vendor Cost Breakdown

## Digi-Key Order

- Sub-Total: **$24.43**
- Tax: $4.71
- Shipping: $6.99
- Tariff: $0.517
- **Digi-Key Grand Total: $31.93**

## Adafruit Order

- Sub-Total: **$34.95**
- Sales Tax: $2.83
- Shipping: $6.17
- **Adafruit Grand Total: $43.95**

---

# Estimated Total Cost (All Vendors)

## Pre-Tax / Pre-Shipping Parts Cost

**$24.43 + $34.95 = $59.38**

## Final Grand Total (Including Tax & Shipping)

**$31.93 + $43.95 = $75.88**

---

## Notes

- Includes power protection (polyfuse + Schottky diode).
- Includes 3.3V switching regulator with inductor and output capacitors.
- Includes hardware debounced switches.
- Includes HDC2080 humidity sensor.
- Includes BNO055 9-DOF orientation IMU (sensor fusion onboard).
- Includes LED debug indicators.
- Includes I2C pull-up resistors.
- Spare passives included for assembly margin.

---
