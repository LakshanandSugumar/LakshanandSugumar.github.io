# API

## Overview

This page defines the messaging interface for the **Sensor + Human–Machine Interface (HMI) subsystem** in the Amphibot V1 system.

This subsystem is responsible for:

- Collecting sensor data (IMU and environmental)
- Processing hazard-related information
- Providing user interaction through buttons and display
- Communicating with other subsystems via UART

All communication uses a **standardized 64-byte UART packet structure**, where message-specific data is contained in the payload.

---

## Subsystem Components

- **BNO055 IMU**
- **HDC2080 Temperature/Humidity Sensor**
- **OLED Display (SPI)**
- **Pushbuttons**
- **Status LEDs**
- **PIC18F47K42 Microcontroller**
- **UART Communication Interface**
- **Microchip Snap (Programming/Debugging)**

---

## Board ID Assignment

| Device           | ID     |
| ---------------- | ------ |
| ESP32 Gateway    | `0x01` |
| Sensor + HMI PIC | `0x02` |
| Actuator PIC     | `0x03` |
| Broadcast        | `0xFF` |

---

## UART Packet Structure (64 Bytes)

| Byte(s) | Field          | Size | Type      | Description             |
| ------- | -------------- | ---- | --------- | ----------------------- |
| 0       | Start Byte     | 1    | `uint8_t` | `0xAA`                  |
| 1       | Message Type   | 1    | `uint8_t` | Defines message         |
| 2       | Sender ID      | 1    | `uint8_t` | Origin device           |
| 3       | Receiver ID    | 1    | `uint8_t` | Destination device      |
| 4       | Payload Length | 1    | `uint8_t` | Number of payload bytes |
| 5–60    | Payload        | 56   | byte[]    | Message data            |
| 61      | Checksum       | 1    | `uint8_t` | Error detection         |
| 62      | Reserved       | 1    | `uint8_t` | Unused                  |
| 63      | End Byte       | 1    | `uint8_t` | `0x55`                  |

---

## Message Types

| Name            | Value  | Description           |
| --------------- | ------ | --------------------- |
| IMU_DATA        | `0x03` | Orientation data      |
| TEMP_HUMIDITY   | `0x05` | Environmental data    |
| BUTTON_EVENT    | `0x08` | Button press          |
| DISPLAY_UPDATE  | `0x07` | Update OLED           |
| STATUS_REQUEST  | `0x09` | Request system status |
| STATUS_RESPONSE | `0x0A` | Return system status  |
| EMERGENCY_STOP  | `0x0C` | Immediate stop        |
| ACK             | `0x0F` | Acknowledgement       |

---

## Message Specifications

---

### IMU_DATA (0x03)

- **Direction:** Sent (Broadcast)
- **Units:** Degrees

| Byte(s) | Field | Type      | Min  | Max | Example |
| ------- | ----- | --------- | ---- | --- | ------- |
| 0–1     | roll  | `int16_t` | -180 | 180 | 10      |
| 2–3     | pitch | `int16_t` | -180 | 180 | -5      |
| 4–5     | yaw   | `int16_t` | -180 | 180 | 45      |

- Payload Size: 6 bytes

---

### TEMP_HUMIDITY (0x05)

- **Direction:** Sent (Broadcast)
- **Units:** °C and %

| Byte(s) | Field       | Type       | Min | Max | Example |
| ------- | ----------- | ---------- | --- | --- | ------- |
| 0–1     | temperature | `int16_t`  | -40 | 125 | 25      |
| 2–3     | humidity    | `uint16_t` | 0   | 100 | 60      |

- Payload Size: 4 bytes

---

### BUTTON_EVENT (0x08)

- **Direction:** Sent (Broadcast)

| Byte(s) | Field     | Type      | Min | Max | Example |
| ------- | --------- | --------- | --- | --- | ------- |
| 0       | button_id | `uint8_t` | 1   | 4   | 2       |
| 1       | state     | `uint8_t` | 0   | 1   | 1       |

- Payload Size: 2 bytes

---

### DISPLAY_UPDATE (0x07)

- **Direction:** Received / Acted On

| Byte(s) | Field        | Type         | Min | Max | Example |
| ------- | ------------ | ------------ | --- | --- | ------- |
| 0       | display_mode | `uint8_t`    | 0   | 5   | 1       |
| 1–5     | data         | `uint8_t[5]` | 0   | 255 | 10      |

- Payload Size: 6 bytes

---

### STATUS_REQUEST (0x09)

- **Direction:** Received

| Byte(s) | Field        | Type |
| ------- | ------------ | ---- |
| —       | (no payload) | —    |

- Payload Size: 0 bytes

---

### STATUS_RESPONSE (0x0A)

- **Direction:** Sent

| Byte(s) | Field       | Type      | Min | Max | Example |
| ------- | ----------- | --------- | --- | --- | ------- |
| 0       | status_code | `uint8_t` | 0   | 5   | 1       |
| 1       | error_flag  | `uint8_t` | 0   | 1   | 0       |

- Payload Size: 2 bytes

---

### EMERGENCY_STOP (0x0C)

- **Direction:** Received / Acted On

| Byte(s) | Field        | Type |
| ------- | ------------ | ---- |
| —       | (no payload) | —    |

- Payload Size: 0 bytes

---

### ACK (0x0F)

- **Direction:** Sent

| Byte(s) | Field      | Type      | Example |
| ------- | ---------- | --------- | ------- |
| 0       | acked_type | `uint8_t` | 0x03    |
| 1       | status     | `uint8_t` | 0x00    |

- Payload Size: 2 bytes

---

## Message Routing Rules

- If `receiver_id == 0x02` → process message
- If `receiver_id != 0x02` → forward message
- If `receiver_id == 0xFF` → process as broadcast
- Messages always continue through the daisy chain

---

## Receiver Behavior

The receiver:

- Validates start (`0xAA`) and end (`0x55`) bytes
- Checks checksum
- Discards malformed or oversized messages
- Ignores self-originated messages
- Processes valid messages addressed to this board
- Forwards all other messages
- Sends ACK for valid processed messages

---

## Sender Behavior

The sender:

- Constructs valid packets using defined format
- Uses correct sender ID (`0x02`)
- Fills payload correctly
- Computes checksum
- Sends periodic sensor data
- Sends event-driven button messages
- Prioritizes forwarding over sending
- Uses non-blocking timing

---

## Example Packet

Example IMU message:

```text
AA 03 02 FF 06 00 0A FF FB 00 2D CS 00 55
```
