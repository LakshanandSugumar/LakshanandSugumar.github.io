---
title: Lessons Learned
---

# Lessons Learned

**Lakshanand Sugumar — Team 302 — EGR314 Spring 2026**

---

1. **Verify MCC Melody clock source configuration before fabricating.** The hardware I²C failure in V1 came down to one register — I2C1CLK set to MFINTOSC instead of FOSC/4. This is not caught by the compiler or linker. The fix is to read the MCC Melody generated code against the datasheet before taping out, not after. An hour of pre-fabrication review would have prevented days of bring-up debugging.

2. **LVP must be disabled in configuration bits for standard ICSP.** The SNAP programmer failed to communicate on first bring-up because LVP was enabled in the configuration bits, which changes the voltage levels expected on the programming pins. This is not documented prominently in the MCC Melody UI. Always explicitly set LVP = OFF in the configuration bits and verify this in the generated config file before programming.

3. **Confirm BNO055 mode pins and I²C address with a bus scan before writing driver code.** PS0, PS1, and ADR must all be tied to GND to select I²C mode at address 0x28. If any of these float, the chip selects UART mode and disappears from the I²C bus entirely with no error indication. The first firmware step for any I²C sensor should be a bus scan that prints all responding addresses — this immediately confirms wiring before any register reads are attempted.

4. **Software bit-bang I²C is a viable fallback and should be in your toolkit.** When hardware I²C failed post-fabrication, having a working software I²C implementation available meant the subsystem was unblocked within a day. Writing bit-bang I²C from scratch under deadline pressure is stressful. Having a tested, portable implementation ready before bring-up begins is a meaningful risk mitigation strategy.

5. **UART baud rate mismatches produce silent failures, not obvious errors.** When the baud rate between the PIC and the ESP32 was mismatched, the UART appeared to be transmitting — the TX line was toggling, the RX buffer was filling — but the data was garbage. No error flag was set. The fix was to scope both TX lines simultaneously and compare bit timing. Always verify baud rate match with a scope or logic analyzer at the start of integration testing, not after assuming the firmware is correct.

6. **OLED framebuffer transfers are slow and block the bus.** A full 128×64 SH1106 framebuffer transfer over software I²C takes several milliseconds. If sensor reads and display writes share the same bus and the same execution context, the sensor sampling rate drops during every display refresh. Schedule display updates on a separate timer or flag, and never transfer the full framebuffer inside a sensor interrupt handler.

7. **Test each I²C device individually before sharing a bus.** In V1, the BNO055, SH1106, and HDC2080 were all connected to the shared bus simultaneously from the first bring-up attempt. When the bus failed to respond, it was unclear which device was causing the issue. The correct approach is to bring up one device at a time, confirm it responds, then add the next. This makes fault isolation trivial.

8. **Pull-up resistor values matter on a shared I²C bus.** With three devices sharing one bus, the combined input capacitance increases and the rise time on SDA and SCL slows down. A single 4.7 kΩ pull-up pair was used in V1 and worked at software I²C speeds, but at higher clock rates this would cause reliability issues. For future designs with multiple I²C devices on one bus, calculate the RC time constant against the target clock rate and select pull-up values accordingly.
