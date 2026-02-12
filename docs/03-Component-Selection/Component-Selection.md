---
title: Module's Selected Major Components
---

# PIC (HMI + Sensor Subsystem) — Component Selection

## 1. OLED Display **(HMI Output)**

### Option 1

| Solution                                                                                                                                                                                                                                                                                             | Pros                                                                                                                                                      | Cons                                                                                                                         |
| ---------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------- | --------------------------------------------------------------------------------------------------------------------------------------------------------- | ---------------------------------------------------------------------------------------------------------------------------- |
| **0.96" 128×64 SSD1306 OLED (SPI/I²C, SMD module)**<br>![SSD1306 OLED](oled1.jpg)<br>Monochrome OLED display, 128×64 resolution, 3.3V compatible<br>Price: ≈ $X.XX/each<br>[Product Page](https://www.adafruit.com/product/326?srsltid=AfmBOoo_LXezKhIH_iu3YWrEneWkTtmVwhwUP5QhVc7BWcPbyktv6GD8)<br> | - Extremely common with strong library support (u8g2, Adafruit)<br>- Very low power consumption<br>- Simple interface (I²C or SPI)<br>- Compact footprint | - Small display area limits UI readability<br>- Limited graphical capability<br>- Some variants ship configured for I²C only |

### Option 2

| Solution                                                                                                                                                                                                                                                                            | Pros                                                                                                                                                                              | Cons                                                                                                           |
| ----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------- | --------------------------------------------------------------------------------------------------------------------------------------------------------------------------------- | -------------------------------------------------------------------------------------------------------------- |
| **1.3" 128×64 SH1106 / SSD1309 SPI OLED (SMD module)**<br>![1.3 OLED](oled2.jpg)<br>Larger monochrome OLED with SPI interface<br>Price: ≈ $X.XX/each<br>[Product Page](PRODUCT_LINK)<https://www.pololu.com/product/3760>[Datasheet](https://www.pololu.com/file/0J1813/SH1106.pdf) | - Larger screen improves readability for hazard alerts<br>- SPI interface allows faster refresh than I²C<br>- Low power (no backlight required)<br>- Good contrast for indoor use | - Slightly larger PCB footprint<br>- Must verify driver compatibility (SH1106 vs SSD1306)<br>- Monochrome only |

### Option 3

| Solution                                                                                                                                                                                                                                                                                                                                           | Pros                                                                                                                                      | Cons                                                                                                       |
| -------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------- | ----------------------------------------------------------------------------------------------------------------------------------------- | ---------------------------------------------------------------------------------------------------------- |
| **1.3" 240×135 ST7789 SPI Color TFT (IPS)**<br>![ST7789 TFT](oled3.jpg)<br>Color TFT display with SPI interface and backlight<br>Price: ≈ $X.XX/each<br>[Product Page](https://www.adafruit.com/product/4313?srsltid=AfmBOopqiBUZGBh_vA-z7tvwGcDS6ydZXt8tBwNHdwKudEyscZ_a9d5M)<br>[Datasheet](https://github.com/adafruit/Adafruit-ST7735-Library) | - Color graphics and better UI visualization<br>- Higher resolution<br>- Wide viewing angles (IPS)<br>- Strong Arduino/PIC driver support | - Higher power draw due to backlight<br>- More complex firmware<br>- Requires backlight current management |

**Choice:** Option 2 — 1.3" SPI OLED

**Rationale:** The 1.3" SPI OLED was selected because it provides the best balance between readability, simplicity, and power consumption for the HMI portion of this subsystem. Since this display is responsible for presenting hazard alerts and system status to the user, clarity is more important than color graphics. The larger 1.3" size improves visibility compared to the smaller 0.96" option without significantly increasing power usage.

Using SPI instead of I²C allows faster screen refresh rates, which is helpful when updating hazard messages or changing display states. Although a color TFT display would allow more advanced graphics, it would also increase power consumption and firmware complexity. For this project, a monochrome OLED is sufficient and keeps the system simpler and more efficient. Therefore, the 1.3" SPI OLED is the most practical and reliable choice.

---

## 2. IMU (Accelerometer + Gyroscope, ≥50 Hz)

### Option 1

| Solution                                                                                                                                                                                                                                                                                                                                                                       | Pros                                                                                                                                            | Cons                                                                      |
| ------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------ | ----------------------------------------------------------------------------------------------------------------------------------------------- | ------------------------------------------------------------------------- |
| **ICM-42688-P (6-Axis IMU, QFN SMD)**<br>![ICM42688](imu1.webp)<br>High-performance accelerometer + gyroscope with I²C/SPI<br>Price: ≈ $X.XX/each<br>[Product Page](https://www.mouser.com/ProductDetail/TDK-InvenSense/ICM-42688-P?qs=OlC7AqGiEDk8QrNAV728zw%3D%3D)<br>[Datasheet](https://invensense.tdk.com/wp-content/uploads/2020/04/ds-000347_icm-42688-p-datasheet.pdf) | - Supports very high sample rates (>1kHz)<br>- Low noise and high precision<br>- FIFO buffering reduces MCU load<br>- Low power modes available | - QFN package is small and harder to solder<br>- No built-in magnetometer |

### Option 2

| Solution                                                                                                                                                                                                                                                                                                                                                                                            | Pros                                                                                                                      | Cons                                                             |
| --------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------- | ------------------------------------------------------------------------------------------------------------------------- | ---------------------------------------------------------------- |
| **BNO055 (9-DOF with onboard fusion)**<br>![BNO055](imu2.jpg)<br>Accelerometer + Gyro + Magnetometer with built-in sensor fusion<br>Price: ≈ $X.XX/each<br>[Product Page](https://www.adafruit.com/product/2472?srsltid=AfmBOoo11VlNEzRJw4b2-O_QUiLeWyB4mrL9w5Jkuk5AHlOwoGJQgGjh)<br>[Datasheet](https://cdn-learn.adafruit.com/assets/assets/000/125/776/original/bst-bno055-ds000.pdf?1698865246) | - Built-in sensor fusion simplifies firmware<br>- Outputs orientation directly<br>- I²C interface<br>- Good documentation | - More expensive<br>- Larger footprint<br>- Fusion startup delay |

### Option 3

| Solution                                                                                                                                                                                                                                                 | Pros                                                                                  | Cons                                                                                                     |
| -------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------- | ------------------------------------------------------------------------------------- | -------------------------------------------------------------------------------------------------------- |
| **MPU-6050 (6-Axis IMU)**<br>![MPU6050](imu3.jpg)<br>Accelerometer + Gyroscope combo with I²C interface<br>Price: ≈ $X.XX/each<br>[Product Page](https://www.adafruit.com/product/3886?srsltid=AfmBOoq__oM6B1grKQ4wIldIZ_-gCz3DHgRXQjVc1gbSGZEj01kumpLN) | - Very common and inexpensive<br>- Strong community support<br>- Simple I²C interface | - Older generation sensor<br>- Higher noise compared to modern parts<br>- Limited long-term availability |

**Choice:** Option 1 — ICM-42688-P

**Rationale:** The ICM-42688-P was selected because it exceeds the required 50 Hz sampling rate while maintaining low noise and good power efficiency. Since this subsystem uses motion data for hazard scoring and system monitoring, consistent and accurate measurements are critical. This sensor provides high sampling headroom, which ensures stable readings even if additional filtering or processing is required.

It also includes a FIFO buffer, which reduces the processing load on the microcontroller and improves timing reliability. Although the QFN package is smaller and slightly more difficult to solder, the performance and long-term reliability outweigh that concern. Compared to older sensors like the MPU-6050, this part offers better precision and modern support. For these reasons, the ICM-42688-P is the most suitable option.

---

## 3. Temperature & Humidity Sensor

### Option 1

| Solution                                                                                                                                                                                                                                                                                                                                                                                                                                                                               | Pros                                                                                                                    | Cons                                                                   |
| -------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------- | ----------------------------------------------------------------------------------------------------------------------- | ---------------------------------------------------------------------- |
| **HDC2080 (TI, I²C Humidity/Temp Sensor)**<br>![HDC2080](th1.webp)<br>Low-power digital humidity and temperature sensor<br>Price: ≈ $X.XX/each<br>[Product Page](https://www.mouser.com/new/texas-instruments/ti-hdc2080-digital-sensor/?srsltid=AfmBOopbs7QEDgVt1Se0jtR92sNIcJDOGL62Ap_O_WdDfbBJbkRJvYPP)<br>[Datasheet](https://www.ti.com/lit/ds/symlink/hdc2080.pdf?HQS=dis-mous-null-mousermode-dsf-pf-null-wwe&ts=1770865467310&ref_url=https%253A%252F%252Fwww.mouser.com%252F) | - Very low power consumption<br>- Good accuracy and stability<br>- I²C interface<br>- Strong manufacturer documentation | - Requires proper PCB layout for accuracy<br>- Needs pull-up resistors |

### Option 2

| Solution                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                       | Pros                                                  | Cons                                                                              |
| ---------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------- | ----------------------------------------------------- | --------------------------------------------------------------------------------- |
| **AHT21 (I²C Humidity/Temp Sensor)**<br>![AHT21](th2.webp)<br>Compact humidity and temperature sensor<br>Price: ≈ $X.XX/each<br>[Product Page](www.google.com/url?sa=t&rct=j&q=&esrc=s&source=web&cd=&ved=2ahUKEwiv2ceG_NKSAxUPJUQIHXTWHqoQFnoECFwQAQ&url=https%3A%2F%2Fwww.eitkw.com%2Fproduct%2Faht21-high-precision-digital-temperature-and-humidity-sensor-measurement-module%2F%3Fsrsltid%3DAfmBOorGya4DsAJMbv9pDby-S7Hc9xnjfZ8UfY15wrXI9VHNEN5t3K3a&usg=AOvVaw3z7l3TLaRAJfeQL3UwHPUO&opi=89978449)<br>[Datasheet](https://www.aosong.com/userfiles/files/media/Data%20Sheet%20AHT21.pdf) | - Low cost<br>- I²C compatible<br>- Good availability | - Slightly less accurate than TI parts<br>- Requires careful voltage verification |

### Option 3

| Solution                                                                                                                                                                                                                                                                                                                   | Pros                                        | Cons                                                                                    |
| -------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------- | ------------------------------------------- | --------------------------------------------------------------------------------------- |
| **DHT11 (Digital Temp/Humidity)**<br>![DHT11](th3.jpg)<br>Single-wire temperature and humidity sensor<br>Price: ≈ $X.XX/each<br>[Product Page](https://www.adafruit.com/product/386?srsltid=AfmBOoouGJmtxsMhO42MX4IPdPwsJjkH9oKYL11U_6k5JMPVDcHP7ZV9)<br>[Datasheet](https://cdn-learn.adafruit.com/downloads/pdf/dht.pdf) | - Very inexpensive<br>- Simple to prototype | - Low accuracy<br>- Timing-sensitive protocol<br>- Not ideal for engineering validation |

**Choice:** Option 1 — HDC2080

**Rationale:** The HDC2080 was selected because it provides reliable and accurate humidity and temperature measurements while consuming very little power. Since environmental data contributes to hazard scoring, measurement stability and accuracy are more important than simply choosing the cheapest option.

This sensor uses I²C communication, which integrates cleanly with the PIC microcontroller and matches the team’s communication strategy. It also has strong documentation and manufacturer support, which reduces design risk compared to lower-cost hobby sensors. While the AHT21 is also a reasonable option, the HDC2080 provides slightly better performance and long-term reliability. Therefore, it is the optimal choice for this subsystem.

---

## 4. User Input (Tactile Switches + LEDs)

### Option 1

| Solution                                                                                                                                                                                                                                                                                                      | Pros                                                                           | Cons                   |
| ------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------- | ------------------------------------------------------------------------------ | ---------------------- |
| **6×6mm SMD Tactile Switch**<br>![TactSwitch](user1.jpg)<br>Surface-mount momentary pushbutton<br>Price: ≈ $X.XX/each<br>[Product Page](https://www.digikey.com/en/product-highlight/s/schurter/6-mm-x-6-mm-tact-switches)<br>[Datasheet](https://www.schurter.com/en/datasheet/typ_6x6_mm_tact_switches.pdf) | - Easy to solder<br>- Clear tactile feedback<br>- Durable for repeated lab use | - Larger PCB footprint |

### Option 2

| Solution                                                                                                                                                                                               | Pros                                            | Cons                                         |
| ------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------ | ----------------------------------------------- | -------------------------------------------- |
| **4×4mm Low-Profile SMD Tact Switch**<br>![SmallSwitch](user2.jpg)<br>Compact surface-mount switch<br>Price: ≈ $X.XX/each<br>[Product Page](https://zzmarts.com/product/smd-tactile-switch-4x4x1-5mm/) | - Saves board space<br>- Clean design aesthetic | - Harder to press<br>- Less tactile feedback |

### Option 3

| Solution                                                                                                                                                                                                                                                                                                                             | Pros                                       | Cons                                                                          |
| ------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------ | ------------------------------------------ | ----------------------------------------------------------------------------- |
| **Capacitive Touch Button (SMD IC + pad)**<br>![CapTouch](user3.jpg)<br>Touch-sensitive input<br>Price: ≈ $X.XX/each<br>[Product Page](https://www.digikey.com/en/products/detail/azoteq-pty-ltd/IQS228B00000000TSR/13663636)<br>[Datasheet](https://mm.digikey.com/Volume0/opasdata/d220001/medias/docus/6161/IQS228B%2CD_v1.2.pdf) | - No mechanical wear<br>- Modern interface | - Requires calibration<br>- Susceptible to noise<br>- Less intuitive feedback |

**Choice:** Option 1 — 6×6mm SMD Tact Switch

**Rationale:** The 6×6mm surface-mount tactile switch was selected because it provides clear physical feedback and is easy to solder during PCB assembly. Since this board will be tested and demonstrated multiple times, durability and ease of use are important considerations.

Smaller switches would save board space but would be harder to press and more difficult to solder reliably. Capacitive touch input was considered, but it would add unnecessary firmware complexity and potential noise issues. A standard tactile switch combined with firmware debouncing provides a simple, reliable solution. For these reasons, the 6×6mm SMD tactile switch is the most practical choice.

---

## 5. 3.3V Switching Regulator

### Option 1

| Solution                                                                                                                                                                                                                                                                                                                                        | Pros                                                                                                                | Cons                                         |
| ----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------- | ------------------------------------------------------------------------------------------------------------------- | -------------------------------------------- |
| **AP63203WU-7 (3.3V Buck Converter)**<br>![AP63203](3.31.jpg)<br>3A SMD switching regulator<br>Price: ≈ $X.XX/each<br>[Product Page](<www.google.com/search?client=opera-gx&q=AP63203WU-7+(3.3V+Buck+Converter)&sourceid=opera&ie=UTF-8&oe=UTF-8>)<br>[Datasheet](https://www.diodes.com/assets/Datasheets/AP63200-AP63201-AP63203-AP63205.pdf) | - High current capability<br>- Surface mount compliant<br>- Good efficiency<br>- Already used in team documentation | - Requires external inductor and layout care |

### Option 2

| Solution                                                                                                                                                                                                                                                                                                                       | Pros                                                                               | Cons                                                        |
| ------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------ | ---------------------------------------------------------------------------------- | ----------------------------------------------------------- |
| **TPS62840 (3.3V Buck Converter)**<br>![TPS62840](3.32.jpg)<br>High-efficiency compact regulator<br>Price: ≈ $X.XX/each<br>[Product Page](https://www.ti.com/product/TPS62840)<br>[Datasheet](https://www.ti.com/lit/ds/symlink/tps62840.pdf?ts=1770841697853&ref_url=https%253A%252F%252Fwww.ti.com%252Fproduct%252FTPS62840) | - Excellent efficiency<br>- Low thermal dissipation<br>- Strong transient response | - Slightly higher cost<br>- Smaller package (harder layout) |

### Option 3

| Solution                                                                                                                                                                                                                                                                            | Pros                                          | Cons                                                                          |
| ----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------- | --------------------------------------------- | ----------------------------------------------------------------------------- |
| **AMS1117-3.3 LDO**<br>![AMS1117](3.33.jpg)<br>Linear voltage regulator<br>Price: ≈ $X.XX/each<br>[Product Page](https://www.digikey.com/en/products/detail/umw/AMS1117-3-3/17635254)<br>[Datasheet](https://mm.digikey.com/Volume0/opasdata/d220001/medias/docus/5011/AMS1117.pdf) | - Very simple design<br>- Low component count | - Poor efficiency<br>- High heat dissipation<br>- Not ideal for dynamic loads |

**Choice:** Option 1 — AP63203WU-7

**Rationale:** The AP63203WU-7 switching regulator was selected because it provides sufficient current capacity with good efficiency while remaining surface-mount compliant. Since this subsystem includes a display, sensors, and microcontroller, stable 3.3V power is essential for reliable operation.

Compared to a linear regulator like the AMS1117, the switching regulator produces significantly less heat and wastes less power, especially under varying load conditions. Although switching regulators require additional external components and careful PCB layout, the improved efficiency and thermal performance make them a better long-term solution. Additionally, this regulator aligns with the team’s existing component selection, which simplifies integration and BOM management. For these reasons, the AP63203WU-7 is the optimal power solution.

---

## Final Component Selection Summary

The table below summarizes the major active components selected for **Laksh’s HMI & Sensor Subsystem**.  
This excludes passive components (resistors, capacitors, inductors), pull-up resistors, small filtering components, and standard PCB hardware.

| **Subsystem**             | **Component**                         | **Manufacturer**     | **Key Specs**                                        | **Price** | **Source** |
| ------------------------- | ------------------------------------- | -------------------- | ---------------------------------------------------- | --------- | ---------- |
| **Microcontroller**       | PIC18F47Q10                           | Microchip Technology | 8-bit MCU, 3.3V logic, I²C/SPI/UART support          | $X.XX     | DigiKey    |
| **OLED Display (HMI)**    | 1.3" 128×64 SPI OLED (SH1106/SSD1309) | (TBD Vendor)         | Monochrome, SPI interface, 3.3V compatible           | $X.XX     | DigiKey    |
| **IMU (Motion Sensor)**   | ICM-42688-P                           | TDK InvenSense       | 6-axis accel + gyro, FIFO buffer, I²C/SPI, >50Hz     | $X.XX     | DigiKey    |
| **Temp/Humidity Sensor**  | HDC2080                               | Texas Instruments    | I²C interface, low power, high accuracy              | $X.XX     | DigiKey    |
| **3.3V Regulation**       | AP63203WU-7                           | Diodes Inc.          | 3A buck converter, high efficiency, SMD              | $X.XX     | DigiKey    |
| **User Input**            | 6×6mm SMD Tactile Switch              | (TBD Vendor)         | Momentary pushbutton, surface-mount                  | $X.XX     | DigiKey    |
| **Status Indicators**     | 0805 SMD LEDs                         | (TBD Vendor)         | 3.3V logic compatible with current-limiting resistor | $X.XX     | DigiKey    |
| **Daisy-Chain Interface** | 2×4 IDC Header (Ribbon Cable)         | (TBD Vendor)         | Standard UART ribbon interface (course requirement)  | $X.XX     | DigiKey    |

---

**Estimated Total Core Component Cost: ≈ $18–$25 per board**  
(excluding passives, PCB fabrication, shipping, and optional spare components)

---

### Cost Discussion

The total cost of Laksh’s subsystem remains reasonable given the number of sensors and interface components required for real-time hazard monitoring and user interaction. The IMU represents one of the higher-cost components due to its precision sensing capabilities and FIFO support, but this cost is justified because motion data reliability is critical for hazard scoring.

The OLED display contributes moderate cost while significantly improving user feedback and system clarity. Compared to color TFT alternatives, the monochrome SPI OLED keeps power consumption and complexity lower while still meeting functional requirements.

The AP63203WU-7 switching regulator adds slight cost compared to basic LDO regulators, but it provides improved efficiency and reduced thermal stress, which increases system reliability. Overall, the selected components balance performance, accuracy, power efficiency, and manufacturability while staying within a reasonable budget for a student-designed embedded subsystem.
