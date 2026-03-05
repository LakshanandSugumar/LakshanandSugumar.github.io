---
title: Module's Selected Major Components
---

# PIC (HMI + Sensor Subsystem) — Component Selection

---

# 1. OLED Display (HMI Output)

## Option 1 — 0.96" 128×64 SSD1306 OLED (SPI/I²C)

![SSD1306 OLED](oled1.jpg)

| Specification | Details                              |
| ------------- | ------------------------------------ |
| Resolution    | 128×64                               |
| Interface     | SPI / I²C                            |
| Voltage       | 3.3V compatible                      |
| Price         | ≈ $2.00                              |
| Product Page  | https://www.adafruit.com/product/326 |

### Pros

- Widely used with extensive software support
- Low power consumption
- Simple communication interface
- Small footprint

### Cons

- Limited screen size reduces readability
- Basic monochrome graphics only
- Some versions default to I²C configuration

---

## Option 2 — 1.3" 128×64 SH1106 / SSD1309 SPI OLED

![1.3 OLED](oled2.jpg)

| Specification | Details                                       |
| ------------- | --------------------------------------------- |
| Resolution    | 128×64                                        |
| Interface     | SPI                                           |
| Voltage       | 3.3V compatible                               |
| Price         | ≈ $6.50                                       |
| Product Page  | https://www.pololu.com/product/3760           |
| Datasheet     | https://www.pololu.com/file/0J1813/SH1106.pdf |

### Pros

- Larger viewing area improves clarity
- Faster refresh rate via SPI
- No backlight required (lower power)
- High contrast for indoor use

### Cons

- Slightly increased PCB area
- Requires driver compatibility verification
- Monochrome display

---

## Option 3 — 1.3" 240×135 ST7789 SPI Color TFT (IPS)

![ST7789 TFT](oled3.jpg)

| Specification | Details                                             |
| ------------- | --------------------------------------------------- |
| Resolution    | 240×135                                             |
| Interface     | SPI                                                 |
| Voltage       | 3.3V logic                                          |
| Price         | ≈ $17.00                                            |
| Product Page  | https://www.adafruit.com/product/4313               |
| Library       | https://github.com/adafruit/Adafruit-ST7735-Library |

### Pros

- Full-color graphical interface
- Higher resolution
- Wide viewing angles

### Cons

- Increased power consumption
- Greater firmware complexity
- Higher overall cost

---

**Selected Display:** Option 2 — 1.3" SPI OLED

**Rationale:**  
The 1.3" SPI OLED was selected because it provides improved readability for hazard and status information while maintaining low power consumption and firmware simplicity. It offers a good balance between visibility, cost, and implementation complexity.

---

# 2. IMU (Accelerometer + Gyroscope, ≥50 Hz)

## Option 1 — ICM-42688-P (6-Axis IMU)

![ICM42688](imu1.webp)

| Specification | Details                                                                                   |
| ------------- | ----------------------------------------------------------------------------------------- |
| Axes          | 6 (Accel + Gyro)                                                                          |
| Interface     | I²C / SPI                                                                                 |
| Sample Rate   | >1 kHz                                                                                    |
| Price         | ≈ $4.70                                                                                   |
| Datasheet     | https://invensense.tdk.com/wp-content/uploads/2020/04/ds-000347_icm-42688-p-datasheet.pdf |

### Pros

- High sampling capability
- Low noise and precision sensing
- FIFO reduces processor overhead
- Power-efficient modes

### Cons

- Small QFN package increases assembly difficulty
- Does not include magnetometer

---

## Option 2 — BNO055 (9-DOF with onboard fusion)

![BNO055](9dofimu.jpg)

| Specification | Details                                                                                           |
| ------------- | ------------------------------------------------------------------------------------------------- |
| Axes          | 9-DOF                                                                                             |
| Interface     | I²C                                                                                               |
| Price         | ≈ $35.00                                                                                          |
| Datasheet     | https://cdn-learn.adafruit.com/assets/assets/000/125/776/original/bst-bno055-ds000.pdf?1698865246 |

### Pros

- Integrated sensor fusion
- Outputs orientation directly
- Reduces firmware development complexity
- Includes magnetometer for full 9-DOF capability

### Cons

- Higher cost
- Larger footprint
- Startup calibration delay

---

## Option 3 — MPU-6050 (6-Axis IMU)

![MPU6050](imu3.jpg)

| Specification | Details |
| ------------- | ------- |
| Axes          | 6-DOF   |
| Interface     | I²C     |
| Price         | ≈ $3.50 |

### Pros

- Cost-effective
- Widely supported
- Simple interface

### Cons

- Older architecture
- Higher noise levels
- Limited future support

---

**Selected IMU:** BNO055 (9-DOF with onboard fusion)

**Rationale:**  
The BNO055 was selected because its built-in sensor fusion significantly simplifies orientation computation and hazard analysis. By providing processed orientation data directly, it reduces firmware complexity on the PIC microcontroller and improves overall system clarity during demonstration and testing.

---

# 3. Temperature & Humidity Sensor

## Option 1 — HDC2080 (TI)

![HDC2080](th1.webp)

| Specification | Details                                       |
| ------------- | --------------------------------------------- |
| Interface     | I²C                                           |
| Voltage       | 1.62–3.6V                                     |
| Price         | ≈ $3.75                                       |
| Datasheet     | https://www.ti.com/lit/ds/symlink/hdc2080.pdf |

---

## Option 2 — AHT21

![AHT21](th2.webp)

| Specification | Details                                                               |
| ------------- | --------------------------------------------------------------------- |
| Interface     | I²C                                                                   |
| Price         | ≈ $2.20                                                               |
| Datasheet     | https://www.aosong.com/userfiles/files/media/Data%20Sheet%20AHT21.pdf |

---

## Option 3 — DHT11

![DHT11](th3.jpg)

| Specification | Details     |
| ------------- | ----------- |
| Interface     | Single-wire |
| Price         | ≈ $1.50     |

---

**Selected Sensor:** HDC2080

**Rationale:**  
The HDC2080 provides higher accuracy and better long-term stability compared to low-cost alternatives. Its I²C interface integrates cleanly with the system and supports reliable environmental monitoring required for hazard evaluation.

---

# 4. User Input (Tactile Switch)

## 6×6mm SMD Tactile Switch

![TactSwitch](user1.jpg)

| Specification | Details                                                            |
| ------------- | ------------------------------------------------------------------ |
| Type          | Momentary pushbutton                                               |
| Mounting      | Surface-mount                                                      |
| Price         | ≈ $0.30                                                            |
| Datasheet     | https://www.schurter.com/en/datasheet/typ_6x6_mm_tact_switches.pdf |

**Rationale:**  
This tactile switch provides reliable mechanical feedback and supports interrupt-driven input handling for local interaction and emergency stop functionality.

---

# 5. 3.3V Switching Regulator

## AP63203WU-7 (3A Buck Converter)

![AP63203](3.31.jpg)

| Specification | Details                                                                      |
| ------------- | ---------------------------------------------------------------------------- |
| Output        | 3.3V                                                                         |
| Current       | 3A                                                                           |
| Price         | ≈ $1.50                                                                      |
| Datasheet     | https://www.diodes.com/assets/Datasheets/AP63200-AP63201-AP63203-AP63205.pdf |

**Selected Regulator:** AP63203WU-7

**Rationale:**  
The AP63203WU-7 was selected because it provides a simple and cost-effective 3.3V solution with sufficient current capacity for system operation. Its fixed output simplifies design while maintaining reliable performance under typical operating loads.

---

# Final Component Selection Summary

| Subsystem       | Component      | Manufacturer | Key Specs             | Price  |
| --------------- | -------------- | ------------ | --------------------- | ------ |
| Microcontroller | PIC18F47Q10    | Microchip    | 8-bit MCU, 3.3V logic | $2.60  |
| OLED Display    | 1.3" SPI OLED  | TBD          | 128×64, SPI           | $6.50  |
| IMU             | BNO055         | Bosch        | 9-DOF, Fusion         | $35.00 |
| Temp/Humidity   | HDC2080        | TI           | I²C, low power        | $3.75  |
| Regulator       | AP63203WU-7    | Diodes Inc.  | 3A Buck               | $1.50  |
| Button          | 6×6mm SMD      | TBD          | Momentary             | $0.30  |
| LEDs            | 0805 SMD       | TBD          | 3.3V compatible       | $0.10  |
| Interface       | 2×4 IDC Header | TBD          | UART ribbon           | $0.50  |

---

## Estimated Total Core Component Cost

**≈ $20–$23 per board**  
(excluding passives, PCB fabrication, and shipping)
