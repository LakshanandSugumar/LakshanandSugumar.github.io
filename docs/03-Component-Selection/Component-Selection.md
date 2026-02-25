---
title: Module Selected Major Components
---

# PIC (HMI + Sensor Subsystem) — Component Selection

---

# 1. OLED Display (HMI Output)

## Option 1 — 0.96" 128×64 SSD1306 OLED

| Specification | Details                              |
| ------------- | ------------------------------------ |
| Resolution    | 128×64                               |
| Interface     | SPI / I²C                            |
| Voltage       | 3.3V compatible                      |
| Price         | ≈ $2.00                              |
| Product Page  | https://www.adafruit.com/product/326 |

### Pros

- Extremely common with strong library support (u8g2, Adafruit)
- Very low power consumption
- Simple interface (I²C or SPI)
- Compact footprint

### Cons

- Small display area limits UI readability
- Limited graphical capability
- Some variants ship configured for I²C only

---

## Option 2 — 1.3" 128×64 SH1106 / SSD1309 SPI OLED

| Specification | Details                                       |
| ------------- | --------------------------------------------- |
| Resolution    | 128×64                                        |
| Interface     | SPI                                           |
| Voltage       | 3.3V compatible                               |
| Price         | ≈ $6.50                                       |
| Product Page  | https://www.pololu.com/product/3760           |
| Datasheet     | https://www.pololu.com/file/0J1813/SH1106.pdf |

### Pros

- Larger screen improves readability for hazard alerts
- SPI allows faster refresh than I²C
- Low power (no backlight required)
- High contrast for indoor environments

### Cons

- Slightly larger PCB footprint
- Must verify SH1106 vs SSD1306 driver compatibility
- Monochrome only

---

## Option 3 — 1.3" 240×135 ST7789 SPI Color TFT (IPS)

| Specification | Details                                             |
| ------------- | --------------------------------------------------- |
| Resolution    | 240×135                                             |
| Interface     | SPI                                                 |
| Voltage       | 3.3V logic                                          |
| Price         | ≈ $17.00                                            |
| Product Page  | https://www.adafruit.com/product/4313               |
| Library       | https://github.com/adafruit/Adafruit-ST7735-Library |

### Pros

- Full color graphics
- Higher resolution
- Wide viewing angles (IPS)
- Strong library support

### Cons

- Higher power draw (backlight required)
- More complex firmware
- Requires backlight current management

---

### Selected Display: Option 2 — 1.3" SPI OLED

**Rationale:**  
Provides improved readability over 0.96" displays while maintaining low power consumption and simple SPI integration. Offers a strong balance between UI clarity, firmware complexity, and energy efficiency.

---

# 2. IMU (Accelerometer + Gyroscope, ≥50 Hz)

## Option 1 — ICM-42688-P (6-Axis IMU)

| Specification | Details                                                                                   |
| ------------- | ----------------------------------------------------------------------------------------- |
| Axes          | 3-Axis Accel + 3-Axis Gyro                                                                |
| Interface     | I²C / SPI                                                                                 |
| Sample Rate   | >1 kHz                                                                                    |
| Price         | ≈ $4.70                                                                                   |
| Datasheet     | https://invensense.tdk.com/wp-content/uploads/2020/04/ds-000347_icm-42688-p-datasheet.pdf |

### Pros

- Very high sample rates
- Low noise, high precision
- FIFO buffering reduces MCU load
- Low power modes available

### Cons

- QFN package (harder to solder)
- No magnetometer

---

## Option 2 — BNO055 (9-DOF with Onboard Fusion)

| Specification | Details                                                                                |
| ------------- | -------------------------------------------------------------------------------------- |
| Axes          | 9-DOF (Accel + Gyro + Mag)                                                             |
| Interface     | I²C                                                                                    |
| Price         | ≈ $12.00                                                                               |
| Datasheet     | https://cdn-learn.adafruit.com/assets/assets/000/125/776/original/bst-bno055-ds000.pdf |

### Pros

- Built-in sensor fusion
- Outputs absolute orientation
- Simplifies firmware

### Cons

- Higher cost
- Larger footprint
- Fusion startup delay

---

## Option 3 — MPU-6050 (6-Axis IMU)

| Specification | Details |
| ------------- | ------- |
| Axes          | 6-DOF   |
| Interface     | I²C     |
| Price         | ≈ $3.50 |

### Pros

- Very common
- Inexpensive
- Strong community support

### Cons

- Older generation
- Higher noise
- Limited long-term availability

---

### Selected IMU: ICM-42688-P

Chosen for superior precision, high sampling capability, and FIFO buffering which reduces MCU processing overhead.

---

# 3. Temperature & Humidity Sensor

## Option 1 — HDC2080 (Texas Instruments)

| Specification | Details                                       |
| ------------- | --------------------------------------------- |
| Interface     | I²C                                           |
| Voltage       | 1.62–3.6V                                     |
| Price         | ≈ $3.75                                       |
| Datasheet     | https://www.ti.com/lit/ds/symlink/hdc2080.pdf |

### Pros

- Very low power
- High accuracy
- Strong documentation
- Industry-grade reliability

### Cons

- Requires proper PCB layout
- Needs I²C pull-ups

---

## Option 2 — AHT21

| Specification | Details                                                               |
| ------------- | --------------------------------------------------------------------- |
| Interface     | I²C                                                                   |
| Price         | ≈ $2.20                                                               |
| Datasheet     | https://www.aosong.com/userfiles/files/media/Data%20Sheet%20AHT21.pdf |

### Pros

- Lower cost
- Good availability
- Compact

### Cons

- Slightly lower accuracy than TI solution
- Voltage verification required

---

## Option 3 — DHT11

| Specification | Details     |
| ------------- | ----------- |
| Interface     | Single-wire |
| Price         | ≈ $1.50     |

### Pros

- Very inexpensive
- Easy for basic prototyping

### Cons

- Low accuracy
- Timing-sensitive protocol
- Not suitable for engineering validation

---

### Selected Sensor: HDC2080

Chosen for accuracy, reliability, and low power operation suitable for engineering validation and real-world deployment.

---

# 4. User Input (Buttons + LEDs)

## Tactile Switch — 6×6mm SMD Pushbutton

| Specification | Details                                                            |
| ------------- | ------------------------------------------------------------------ |
| Type          | Momentary                                                          |
| Mounting      | Surface Mount                                                      |
| Price         | ≈ $0.30                                                            |
| Datasheet     | https://www.schurter.com/en/datasheet/typ_6x6_mm_tact_switches.pdf |

### Pros

- Durable
- Clear tactile feedback
- Easy to solder

### Cons

- Larger PCB footprint than ultra-miniature variants

---

# 5. 3.3V Switching Regulator

## AP63203WU-7 (3A Buck Converter)

| Specification | Details                                                                      |
| ------------- | ---------------------------------------------------------------------------- |
| Output        | 3.3V                                                                         |
| Current       | 3A                                                                           |
| Price         | ≈ $1.50                                                                      |
| Datasheet     | https://www.diodes.com/assets/Datasheets/AP63200-AP63201-AP63203-AP63205.pdf |

### Pros

- High efficiency
- 3A capability
- Good thermal performance

### Cons

- Requires external inductor
- Careful PCB layout required

---

# Final Component Selection Summary

| Subsystem       | Component                      | Manufacturer | Key Specs                 | Price |
| --------------- | ------------------------------ | ------------ | ------------------------- | ----- |
| Microcontroller | PIC18F47Q10                    | Microchip    | 8-bit, 3.3V, I²C/SPI/UART | $2.60 |
| OLED Display    | 1.3" SPI OLED (SH1106/SSD1309) | TBD          | 128×64, SPI               | $6.50 |
| IMU             | ICM-42688-P                    | TDK          | 6-axis, FIFO, >50Hz       | $4.70 |
| Temp/Humidity   | HDC2080                        | TI           | I²C, Low Power            | $3.75 |
| Regulator       | AP63203WU-7                    | Diodes Inc.  | 3A Buck                   | $1.50 |
| Buttons         | 6×6mm SMD Switch               | TBD          | Momentary                 | $0.30 |
| LEDs            | 0805 SMD                       | TBD          | 3.3V compatible           | $0.10 |
| Interface       | 2×4 IDC Header                 | TBD          | UART Ribbon               | $0.50 |

---

## Estimated Core Component Cost

**≈ $20–$23 per board**  
(Excludes passives, PCB fabrication, shipping, and spares)

---

# Cost Discussion

The subsystem cost remains reasonable considering the sensing accuracy, real-time motion capture capability, and user interface requirements.

The IMU represents a higher-cost component due to its precision and FIFO support, which significantly reduces MCU processing load and improves reliability.

The selected 1.3" SPI OLED provides strong readability while keeping power consumption and firmware complexity manageable compared to full-color TFT options.

The AP63203 buck regulator improves efficiency and thermal performance compared to LDO solutions, increasing overall system reliability.

Overall, the selected components provide a strong balance between:

- Performance
- Accuracy
- Power efficiency
- Manufacturability
- Budget constraints

while remaining appropriate for a student-designed embedded subsystem.
