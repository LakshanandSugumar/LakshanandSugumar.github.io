---
title: Hardware Version 2.0
---

# Hardware Version 2.0

**Lakshanand Sugumar — Team 302 — EGR314 Spring 2026**

---

If a second version of this subsystem were built, the following changes would be made based on what was learned during V1 development and demonstration.

**Display upgrade.** The SH1106 1.3" OLED was readable up close but difficult to see from more than a meter away during the showcase. V2 would use a small TFT LCD — a 1.8" ST7789 or similar — which offers higher brightness, color-coded hazard levels, and a much larger readable distance. Color would allow immediate visual distinction between safe (green), warning (yellow), and hazard (red) states without the operator needing to read numeric values.

**Buzzer for audio fault feedback.** The showcase environment was noisy and visitors sometimes missed the OLED fault indicator. V2 would add a piezo buzzer driven by a PWM pin so that emergency stop events and out-of-range sensor readings produce an audible alert. This makes the system usable in environments where the operator cannot watch the display continuously.

**Dedicated hardware I²C buses.** The hardware I²C conflict in V1 was caused by incorrect MCC Melody clock source configuration — specifically, the I2C1CLK register was left at MFINTOSC instead of FOSC/4. In V2 this would be caught during schematic review by verifying the MCC configuration against the datasheet before fabrication, and the hardware I²C peripheral would be used instead of the software bit-bang fallback. This would increase the bus clock rate and reduce CPU overhead during sensor reads.

**Separate I²C buses for OLED and IMU.** In V1 all three I²C devices shared one software bus, which meant the display framebuffer transfer blocked sensor reads during the write cycle. V2 would use two independent I²C buses — one for the BNO055 and HDC2080, one for the display — so sensor sampling is never delayed by a display update.

**Hazard score display.** The scaled hazard score was computed correctly in firmware but dropped from the OLED rendering under timing pressure. V2 would dedicate one OLED page entirely to the hazard score with a large font and color bar, making it the primary operator-facing output during hazardous environment scouting.

**Larger ICSP test points.** The ICSP header pads on the V1 PCB were small and difficult to probe during bring-up. V2 would add larger labeled test points for PGC, PGD, MCLR, VDD, and GND adjacent to the header so a scope probe can be placed without risk of bridging pads.
