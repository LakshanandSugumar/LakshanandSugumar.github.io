# My Subsystem: Infrared Distance Sensing

I owned the infrared motion-sensing subsystem end to end, from concept to working prototype.

## What it does

The IR subsystem watches both sides of the door and tells the motor board when someone approaches, so the door can open hands-free. It also gives the system a second way to notice something in the door's path.

## How I built it

| Stage | Work |
| --- | --- |
| Sensing front end | Designed the infrared sensing front end for the door's approach zone |
| Signal conditioning | Designed the conditioning circuit that shapes the raw sensor output into a clean, reliable threshold signal for the microcontroller |
| Schematic & PCB | Produced the schematic, fabricated the PCB and executed hardware bring-up |
| Firmware | Wrote the control firmware in embedded C on a PIC18F57Q43 |
| Integration | Connected to the motor hub over the team's 8-wire ribbon-cable interface |
| Verification | Validated the sensing-to-actuation path end to end |

## Result

I defended the design in an external design review and scored **100%** on functionality and documentation.

## Skills used

PIC18F57Q43 · infrared sensing · analog signal conditioning · schematic capture · PCB fabrication and bring-up · embedded C · system integration
