# Firmware Overview

The Sensor + HMI firmware runs on the PIC18F57Q83 and is written in C with MPLAB X and MCC Melody.

| Module | What it does |
| --- | --- |
| Software I²C | Bit-banged I²C master shared by the BNO055, SH1106 and HDC2080 after hardware I²C was blocked post-fabrication |
| BNO055 driver | NDOF fusion mode setup and Euler-angle parsing (heading / roll / pitch) |
| SH1106 driver | 128×64 framebuffer with four switchable telemetry pages |
| HDC2080 driver | Temperature and humidity measurement |
| UART protocol | Builds, parses, routes and relays AZ…YB framed packets between the gateway ('M') and the actuator board ('R') |
| HMI | Pushbutton page switching, status LEDs |
| Safety | Acts on emergency-stop broadcasts immediately and acknowledges them |

## Packet framing

Every packet starts with the ASCII header `AZ` (0x41 0x5A) and ends with the footer `YB` (0x59 0x42), with single-character source and destination IDs:

```
'A' 'Z' | src | dest | payload ... | 'Y' 'B'
```

For example, `AZLMFWDYB` is a forward command from the sensor board ('L') addressed to the gateway ('M'). Board IDs: `M` = ESP32 gateway, `L` = sensor + HMI, `R` = actuator board.

The complete packet and message reference is on the [API & UART protocol](api.md) page.

## Bring-up notes

- Verify the MCC Melody I²C clock source (FOSC/4) against the datasheet before fabrication.
- Set LVP = OFF in the configuration bits for standard ICSP with the SNAP programmer.
- Tie BNO055 PS0, PS1 and ADR low for I²C mode at address 0x28, and run a bus scan before writing any driver code.
- Check UART baud-rate match with a scope or logic analyzer at the start of integration.

More in [Lessons learned](../lessons/lessons-learned.md).
