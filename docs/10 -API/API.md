---
title: API & UART Protocol
---

# Sensor & HMI Subsystem — API & UART Protocol

**Lakshanand Sugumar — Team 302 — EGR314 Spring 2026**

---

## Overview

This page defines the messaging interface for the Sensor & HMI subsystem of the R6 Recon Amphibot. The subsystem is built around a **PIC18F57Q83** and sits in the middle of the three-node shared UART bus. It reads IMU and environmental data, displays telemetry on the OLED, relays motor commands from Mihir to Raunak, and publishes sensor data back to Mihir for MQTT forwarding to the cloud dashboard.

All packets use the team-standardized AZ/YB framing with ASCII payloads. The packet structure, node IDs, and payload formats on this page are consistent with the full team API reference.

---

## Node IDs

| Node                      | ID char | Hex    | Role       |
| ------------------------- | ------- | ------ | ---------- |
| Lakshanand — Sensor + HMI | `L`     | `0x4C` | This board |
| Mihir — WiFi Gateway      | `M`     | `0x4D` | Upstream   |
| Raunak — Actuator         | `R`     | `0x52` | Downstream |
| Broadcast — all nodes     | `X`     | `0x58` | Broadcast  |

---

## UART Configuration

| Parameter          | Value                                                      |
| ------------------ | ---------------------------------------------------------- |
| Baud rate          | 9600 bps                                                   |
| Data bits          | 8                                                          |
| Parity             | None                                                       |
| Stop bits          | 1                                                          |
| Packet length      | Variable (6 bytes minimum, 64 bytes maximum)               |
| Header             | `0x41 0x5A` ('A' 'Z')                                      |
| Footer             | `0x59 0x42` ('Y' 'B')                                      |
| Inter-packet gap   | ≥ 100 ms between transmissions                             |
| Inter-byte timeout | 2 ms (Raunak's receiver uses this to detect end of packet) |

---

## Packet Structure

```
[ 0x41 | 0x5A | src_id | dest_id | payload bytes | 0x59 | 0x42 ]
```

| Field   | Size     | Description                                         |
| ------- | -------- | --------------------------------------------------- |
| Header  | 2 bytes  | Always `0x41 0x5A` (AZ)                             |
| src_id  | 1 byte   | ASCII ID of sending node                            |
| dest_id | 1 byte   | ASCII ID of destination node (or `X` for broadcast) |
| Payload | Variable | ASCII command string or sensor data string          |
| Footer  | 2 bytes  | Always `0x59 0x42` (YB)                             |

Packets where `src_id == MY_ID` are discarded (bus echo). Packets where `dest_id != 'L'` and `dest_id != 'X'` are forwarded without processing.

---

## Message Responsibility Summary

### Sent by This Board

| Dest | Payload        | Trigger                          | Description                   |
| ---- | -------------- | -------------------------------- | ----------------------------- |
| `R`  | `FWD`          | On receipt of FWD from Mihir     | Relay motor forward to Raunak |
| `R`  | `RES`          | On receipt of RES from Mihir     | Relay motor reverse to Raunak |
| `R`  | `STP`          | On receipt of STP from Mihir     | Relay motor stop to Raunak    |
| `M`  | `H:xR:xP:xT:x` | Every 5 s, and on `DATA` request | IMU sensor data string        |
| `M`  | `PAGE{n}`      | On button press                  | OLED page changed to n        |
| `M`  | `ACK`          | On receipt of `ESTOP` broadcast  | Acknowledge emergency stop    |

### Received and Acted On

| Src | Payload | Action                                                  |
| --- | ------- | ------------------------------------------------------- |
| `M` | `FWD`   | Build and send `L→R FWD` packet                         |
| `M` | `RES`   | Build and send `L→R RES` packet                         |
| `M` | `STP`   | Build and send `L→R STP` packet                         |
| `M` | `DATA`  | Read BNO055 + HDC2080, send sensor data string to Mihir |
| `X` | `ESTOP` | Display ESTOP screen on OLED, send ACK to Mihir         |

---

## Payload Specifications

### Sensor Data — `H:xR:xP:xT:x`

**Direction:** Sent to Mihir (`M`)  
**Trigger:** Every 5 seconds, and immediately on `DATA` request  
**Format:** ASCII string

```
H:{heading}R:{roll}P:{pitch}T:{temp}
```

| Field | Source                         | Units               | Example  |
| ----- | ------------------------------ | ------------------- | -------- |
| H     | BNO055 EULER_H register (0x1A) | Degrees (raw ÷ 16)  | `H:45.2` |
| R     | BNO055 EULER_R register (0x1C) | Degrees (raw ÷ 16)  | `R:1.3`  |
| P     | BNO055 EULER_P register (0x1E) | Degrees (raw ÷ 16)  | `P:-2.5` |
| T     | BNO055 TEMP register (0x34)    | °C (1 byte, direct) | `T:28`   |

Full example packet (hex):

```
41 5A 4C 4D 48 3A 34 35 2E 32 52 3A 31 2E 33 50 3A 2D 32 2E 35 54 3A 32 38 59 42
```

Decoded: `AZ L M H:45.2R:1.3P:-2.5T:28 YB`

Mihir parses this string and publishes JSON to `egr314/team302/sensor` over MQTT.

---

### Motor Relay — `FWD` / `RES` / `STP`

**Direction:** Received from Mihir (`M`), relayed to Raunak (`R`)  
**This board's role:** Relay only — does not drive motors directly

| Payload | Mihir → Laksh   | Laksh → Raunak  | Raunak SPI byte |
| ------- | --------------- | --------------- | --------------- |
| `FWD`   | `AZ M L FWD YB` | `AZ L R FWD YB` | `0xEF`          |
| `RES`   | `AZ M L RES YB` | `AZ L R RES YB` | `0xED`          |
| `STP`   | `AZ M L STP YB` | `AZ L R STP YB` | `0xFF`          |

---

### Page Change — `PAGE{n}`

**Direction:** Sent to Mihir (`M`)  
**Trigger:** BTN_PAGE press cycles OLED page

| Value | Payload | OLED Content                                                  |
| ----- | ------- | ------------------------------------------------------------- |
| 0     | `PAGE0` | Heading, Roll, Pitch in degrees                               |
| 1     | `PAGE1` | Temperature °C from BNO055                                    |
| 2     | `PAGE2` | Accelerometer (Ax Ay Az) and Gyroscope (Gx Gy Gz)             |
| 3     | `PAGE3` | System status — Mihir/Raunak online or offline (15 s timeout) |

---

### Emergency Stop — `ESTOP`

**Direction:** Received — broadcast from any node (`X`)  
**Trigger:** Remote dashboard command or WiFi loss on Mihir's side

On receipt:

1. Display ESTOP screen on OLED immediately
2. Send `AZ L M ACK YB` to Mihir
3. Do not re-forward — Raunak receives the broadcast directly on the shared bus

Example packet (hex):

```
41 5A 4D 58 45 53 54 4F 50 59 42
```

Decoded: `AZ M X ESTOP YB`

---

### Data Request — `DATA`

**Direction:** Received from Mihir (`M`)  
**Trigger:** Mihir requests latest sensor reading on demand

On receipt: immediately read BNO055 and HDC2080, build and send `H:xR:xP:xT:x` string to Mihir.

---

## Hardware Pin Reference — PIC18F57Q83

| Pin | Signal             | Direction | Connected To                                     |
| --- | ------------------ | --------- | ------------------------------------------------ |
| RC3 | I²C SCL (bit-bang) | Output    | BNO055 clock, SH1106 clock, HDC2080 clock        |
| RC4 | I²C SDA (bit-bang) | Bidir     | BNO055 data, SH1106 data, HDC2080 data           |
| RC6 | UART1 TX           | Output    | Shared bus transmit                              |
| RC7 | UART1 RX           | Input     | Shared bus receive                               |
| RB0 | BTN_PAGE           | Input     | Cycle OLED pages (10 kΩ pull-up, active LOW)     |
| RB1 | BTN_CAL            | Input     | Recalibrate IMU zero (10 kΩ pull-up, active LOW) |
| RA2 | LED1               | Output    | Status LED 1 (220 Ω series resistor)             |
| RA3 | LED2               | Output    | Status LED 2 (220 Ω series resistor)             |
| RB6 | PGC                | Input     | SNAP programmer clock                            |
| RB7 | PGD                | Bidir     | SNAP programmer data                             |
| RE3 | MCLR               | Input     | 10 kΩ pull-up to 3.3 V                           |

---

## BNO055 Register Reference

| Register | Address | Description                             |
| -------- | ------- | --------------------------------------- |
| CHIP_ID  | 0x00    | Always 0xA0 — verify on init            |
| OPR_MODE | 0x3D    | Set to 0x0C for NDOF fusion mode        |
| PWR_MODE | 0x3E    | Set to 0x00 for normal power            |
| SYS_TRIG | 0x3F    | Write 0x20 to reset                     |
| EULER_H  | 0x1A    | Heading LSB (raw ÷ 16 = degrees)        |
| EULER_R  | 0x1C    | Roll LSB (raw ÷ 16 = degrees)           |
| EULER_P  | 0x1E    | Pitch LSB (raw ÷ 16 = degrees)          |
| ACCEL    | 0x08    | Linear accel X/Y/Z (raw ÷ 100 = m/s²)   |
| GYRO     | 0x14    | Angular velocity X/Y/Z (raw ÷ 16 = °/s) |
| TEMP     | 0x34    | Temperature in °C (1 byte, direct)      |

---

## Routing Rules

| Condition                                         | Action                             |
| ------------------------------------------------- | ---------------------------------- |
| `src_id == 'L'` (own echo)                        | Discard immediately                |
| `dest_id == 'L'`                                  | Process locally                    |
| `dest_id == 'X'` (broadcast)                      | Process locally, do not re-forward |
| `dest_id == 'R'` and payload is `FWD`/`RES`/`STP` | Build new packet `L→R`, send       |
| `dest_id` is anything else                        | Forward without modification       |
| Bad header or footer                              | Discard                            |
| Packet length > 64 bytes                          | Discard                            |

---

## Communication Flow Examples

### Motor Forward (Cloud → Motor)

```
Dashboard → MQTT: egr314/team302/motor = "FWD"
Mihir builds:  AZ M L FWD YB  → sends on UART bus
Laksh receives, dest=L, payload=FWD
Laksh builds:  AZ L R FWD YB  → sends on UART bus
Raunak receives, sends SPI 0xEF → motor drives forward
```

### Sensor Data (IMU → Cloud)

```
Laksh reads BNO055 every 200 ms, caches values
Every 5 s: Laksh builds AZ L M H:45.2R:1.3P:-2.5T:28 YB
Mihir receives, parses string, publishes JSON to egr314/team302/sensor
Dashboard receives sensor data via MQTT subscription
```

### Emergency Stop

```
Dashboard → MQTT: egr314/team302/estop
Mihir builds: AZ M X ESTOP YB (broadcast)
Laksh receives broadcast: shows ESTOP on OLED, sends AZ L M ACK YB
Raunak receives broadcast: sends SPI 0xFF, motor stops
```

---

## Downloads

- [Team API Reference (.docx)](EGR314_Team302_API_Reference.docx)
- [Firmware Source — main.c](main.c)
