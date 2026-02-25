---
title: Module Selected Major Components
---

# PIC (HMI + Sensor Subsystem) — Component Selection

---

# 1. OLED Display (HMI Output)

## Option 1 — 0.96" 128×64 SSD1306 OLED

| Specification | Details                                                                      |
| ------------- | ---------------------------------------------------------------------------- |
| Resolution    | 128×64                                                                       |
| Interface     | SPI / I²C                                                                    |
| Voltage       | 3.3V compatible                                                              |
| Price         | ≈ $2.00                                                                      |
| Image         | ![SSD1306 OLED](https://cdn.sparkfun.com//assets/parts/1/2/9/7/16038-01.jpg) |
| Product Page  | https://www.adafruit.com/product/326                                         |

### Pros

- Very common & excellent library support
- Low power
- Simple interface (I²C/SPI)

### Cons

- Small area limits readability
- Limited graphics
- Some I²C-only variants

---

## Option 2 — 1.3" 128×64 SH1106 / SSD1309 SPI OLED

| Specification | Details                                                                            |
| ------------- | ---------------------------------------------------------------------------------- |
| Resolution    | 128×64                                                                             |
| Interface     | SPI                                                                                |
| Voltage       | 3.3V compatible                                                                    |
| Price         | ≈ $6.50                                                                            |
| Image         | ![1.3" OLED](https://cdn.sparkfun.com//assets/parts/1/3/9/6/16055-SFE12998-01.jpg) |
| Product Page  | https://www.pololu.com/product/3760                                                |
| Datasheet     | https://www.pololu.com/file/0J1813/SH1106.pdf                                      |

### Pros

- Larger screen: better readability
- SPI refresh faster than I²C
- No backlight required
- Good indoor contrast

### Cons

- Larger PCB footprint
- Must verify SH1106/SSD1306 driver
- Monochrome only

---

## Option 3 — 1.3" 240×135 ST7789 SPI Color TFT

| Specification | Details                                                                                 |
| ------------- | --------------------------------------------------------------------------------------- |
| Resolution    | 240×135                                                                                 |
| Interface     | SPI                                                                                     |
| Voltage       | 3.3V                                                                                    |
| Price         | ≈ $17.00                                                                                |
| Image         | ![ST7789 TFT](https://cdn.sparkfun.com//assets/parts/1/4/2/7/28645-BTF-SPI-Display.jpg) |
| Product Page  | https://www.adafruit.com/product/4313                                                   |
| Library       | https://github.com/adafruit/Adafruit-ST7735-Library                                     |

### Pros

- Full color graphics
- Higher resolution
- Wide viewing angles

### Cons

- Higher power (backlight)
- More complex firmware

---

### Selected Display: Option 2 — 1.3" SPI OLED

Balanced readability, low power, and simple firmware.

---

# 2. IMU (Accelerometer + Gyroscope)

## Option 1 — ICM-42688-P

| Specification | Details                                                                                   |
| ------------- | ----------------------------------------------------------------------------------------- |
| Axes          | 6-axis (Accel + Gyro)                                                                     |
| Interface     | SPI / I²C                                                                                 |
| Price         | ≈ $4.70                                                                                   |
| Image         | ![ICM-42688](https://cdn.sparkfun.com//assets/parts/1/3/5/5/28275-IMU_ICM_42688_P.jpg)    |
| Datasheet     | https://invensense.tdk.com/wp-content/uploads/2020/04/ds-000347_icm-42688-p-datasheet.pdf |

### Pros

- High sample rates
- Low noise
- FIFO buffer

### Cons

- QFN (harder solder)
- No magnetometer

---

## Option 2 — BNO055

| Specification | Details                                                                                |
| ------------- | -------------------------------------------------------------------------------------- |
| Axes          | 9-DOF                                                                                  |
| Interface     | I²C                                                                                    |
| Price         | ≈ $12.00                                                                               |
| Image         | ![BNO055](https://cdn.sparkfun.com//assets/parts/1/8/8/1/24751-BNO055.jpg)             |
| Datasheet     | https://cdn-learn.adafruit.com/assets/assets/000/125/776/original/bst-bno055-ds000.pdf |

### Pros

- Built-in fusion
- Orientation data

### Cons

- Higher cost
- Startup delay

---

## Option 3 — MPU-6050

| Specification | Details                                                                        |
| ------------- | ------------------------------------------------------------------------------ |
| Axes          | 6-DOF                                                                          |
| Interface     | I²C                                                                            |
| Price         | ≈ $3.50                                                                        |
| Image         | ![MPU-6050](https://cdn.sparkfun.com//assets/parts/1/1/3/6/13879-MPU-6050.jpg) |

### Pros

- Cheap
- Common

### Cons

- Older tech
- Higher noise

---

### Selected IMU: ICM-42688-P

---

# 3. Temperature & Humidity

## Option 1 — HDC2080

| Specification | Details                                                                      |
| ------------- | ---------------------------------------------------------------------------- |
| Interface     | I²C                                                                          |
| Price         | ≈ $3.75                                                                      |
| Image         | ![HDC2080](https://cdn.sparkfun.com//assets/parts/1/5/8/6/29469-HDC2080.jpg) |
| Datasheet     | https://www.ti.com/lit/ds/symlink/hdc2080.pdf                                |

### Pros

- Low power
- High accuracy

### Cons

- Needs careful layout

---

## Option 2 — AHT21

| Specification | Details                                                                  |
| ------------- | ------------------------------------------------------------------------ |
| Interface     | I²C                                                                      |
| Price         | ≈ $2.20                                                                  |
| Image         | ![AHT21](https://cdn.sparkfun.com//assets/parts/1/9/8/1/28721-AHT21.jpg) |
| Datasheet     | https://www.aosong.com/userfiles/files/media/Data%20Sheet%20AHT21.pdf    |

### Pros

- Low cost
- Compact

### Cons

- Slightly less accurate

---

## Option 3 — DHT11

| Specification | Details                                                                  |
| ------------- | ------------------------------------------------------------------------ |
| Interface     | Single-wire                                                              |
| Price         | ≈ $1.50                                                                  |
| Image         | ![DHT11](https://cdn.sparkfun.com//assets/parts/1/3/5/8/13576-DHT11.jpg) |

### Pros

- Very cheap

### Cons

- Low accuracy
- Timing-sensitive

---

### Selected Temp/Humidity: HDC2080

---

# 4. User Input

## 6×6mm SMD Tactile Switch

| Specification | Details                                                                                   |
| ------------- | ----------------------------------------------------------------------------------------- |
| Type          | Momentary                                                                                 |
| Mount         | SMD                                                                                       |
| Price         | ≈ $0.30                                                                                   |
| Image         | ![6x6mm Tactile](https://cdn.sparkfun.com//assets/parts/1/1/8/7/11877-Tactile_Switch.jpg) |
| Datasheet     | https://www.schurter.com/en/datasheet/typ_6x6_mm_tact_switches.pdf                        |

---

# 5. 3.3V Switching Regulator

## AP63203WU-7 Buck Converter

| Specification | Details                                                                                    |
| ------------- | ------------------------------------------------------------------------------------------ |
| Output        | 3.3V                                                                                       |
| Current       | 3A                                                                                         |
| Price         | ≈ $1.50                                                                                    |
| Image         | ![Buck Converter](https://cdn.sparkfun.com//assets/parts/1/7/5/6/17566-Buck-Converter.jpg) |
| Datasheet     | https://www.diodes.com/assets/Datasheets/AP63200-AP63201-AP63203-AP63205.pdf               |

---

# Final Component Selection Summary

| Subsystem       | Component        | Key Specs                 | Price |
| --------------- | ---------------- | ------------------------- | ----- |
| Microcontroller | PIC18F47Q10      | 8-bit, 3.3V, I²C/SPI/UART | $2.60 |
| OLED Display    | 1.3" SPI OLED    | 128×64, SPI               | $6.50 |
| IMU             | ICM-42688-P      | 6-axis, FIFO              | $4.70 |
| Temp/Humidity   | HDC2080          | I²C, low power            | $3.75 |
| Regulator       | AP63203WU-7      | 3A buck                   | $1.50 |
| Buttons         | 6×6mm SMD Switch | Momentary                 | $0.30 |
| LEDs            | 0805 SMD         | 3.3V logic                | $0.10 |
| Interface       | 2×4 IDC Header   | UART ribbon               | $0.50 |

---

## Estimated Core Cost

**≈ $20–$23 / board** (excl. passives + PCB + shipping)

---
