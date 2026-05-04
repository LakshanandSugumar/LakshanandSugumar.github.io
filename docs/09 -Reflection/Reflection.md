---
title: Reflection
---

# Reflection — Review of Module's Success

**Lakshanand Sugumar — Team 302 — EGR314 Spring 2026**

---

The Sensor & HMI subsystem met its core objectives for the Innovation Showcase. The BNO055 IMU delivered stable Euler angle output in NDOF fusion mode throughout the demonstration, the SH1106 OLED displayed live telemetry across four switchable pages, and the UART bus successfully carried sensor data to Mihir's ESP32 and relayed motor commands downstream to Raunak's actuator board. Both pushbuttons functioned correctly with no false triggers, and the HDC2080 contributed temperature and humidity data to the sensor payload throughout testing and the showcase.

The biggest technical challenge of the project was the hardware I²C peripheral conflict discovered after PCB fabrication. The MSSP peripheral on the PIC18F57K42 could not be brought up reliably due to a configuration issue in the MCC Melody-generated initialization code. Rather than waiting for a board respin, a software bit-bang I²C library was written and substituted with no functional impact — the BNO055, SH1106, and HDC2080 all communicated correctly on the software bus at the reduced clock rate. This was the single decision that unblocked the entire subsystem and allowed integration testing to proceed on schedule.

The hazard score was computed in firmware and included in the UART payload, but was not consistently rendered on the OLED during the showcase due to display refresh timing constraints. This is the one area where the subsystem fell short of its target — the scaled 0–100 score was available in the data stream but not always visible on the display. Everything else performed at or above the target level defined in the requirements.
