# API

## Overview

This page defines the messaging interface for the Sensor + Human–Machine Interface (HMI) subsystem in the Amphibot V1 system.

This subsystem is responsible for:

- Collecting sensor data (IMU and environmental)
- Processing hazard-related information
- Providing user interaction through buttons and display
- Communicating with other subsystems via UART

The system uses a standardized UART packet structure, where message data is contained within the payload portion of each packet.

---

## Subsystem Components

This subsystem includes:

- **BNO055 IMU** (orientation and motion sensing)
- **HDC2080** (temperature and humidity sensing)
- **OLED display** (SPI interface)
- **Pushbuttons** (user input)
- **Status LEDs**
- **PIC18F47K42 microcontroller**
- **UART communication interface**

---

## Subsystem Responsibilities

### Messages Sent

- IMU orientation data
- Temperature and humidity data
- Button press events
- Status responses

### Messages Received

- Display update commands
- Status requests
- Broadcast system messages

### Messages Acted On

- Emergency stop
- System-wide alerts and coordination messages

---

## Message Handling Requirements

### Receiver Behavior

The receiver must:

- Ignore data outside valid message frames
- Ignore malformed or oversized messages
- Forward messages not addressed to this subsystem
- Process messages addressed to this subsystem
- Discard messages originating from itself
- Generate a unique acknowledgment for valid messages

### Sender Behavior

The sender must:

- Format all messages according to protocol
- Use valid prefix and suffix bytes
- Ensure message size is within limits
- Prevent reserved bytes from appearing in payload
- Use time-varying data
- Prioritize forwarding over sending
- Use non-blocking timing control

---

## Message Specifications

---

### Message Type 3 -- IMU Data

#### Message Summary

- **Direction:** Sent (Broadcast)
- **Purpose:** Transmits orientation data from the IMU for system awareness and hazard evaluation.

| Field         | Byte 1–2       | Byte 3–4  | Byte 5–6  | Byte 7–8  |
| ------------- | -------------- | --------- | --------- | --------- |
| Variable Name | `message_type` | `roll`    | `pitch`   | `yaw`     |
| Variable Type | `uint16_t`     | `int16_t` | `int16_t` | `int16_t` |
| Min Value     | 3              | -180      | -180      | -180      |
| Max Value     | 3              | 180       | 180       | 180       |
| Example       | 3              | 10        | -5        | 45        |

- Number of bytes: 8

---

### Message Type 5 -- Temperature & Humidity Data

#### Message Summary

- **Direction:** Sent (Broadcast)
- **Purpose:** Sends environmental data for monitoring and hazard assessment.

| Field         | Byte 1–2       | Byte 3–4      | Byte 5–6   |
| ------------- | -------------- | ------------- | ---------- |
| Variable Name | `message_type` | `temperature` | `humidity` |
| Variable Type | `uint16_t`     | `int16_t`     | `uint16_t` |
| Min Value     | 5              | -40           | 0          |
| Max Value     | 5              | 125           | 100        |
| Example       | 5              | 25            | 60         |

- Number of bytes: 6

---

### Message Type 8 -- Button Event

#### Message Summary

- **Direction:** Sent (Broadcast)
- **Purpose:** Reports user input from pushbuttons.

| Field         | Byte 1–2       | Byte 3      | Byte 4    |
| ------------- | -------------- | ----------- | --------- |
| Variable Name | `message_type` | `button_id` | `state`   |
| Variable Type | `uint16_t`     | `uint8_t`   | `uint8_t` |
| Min Value     | 8              | 1           | 0         |
| Max Value     | 8              | 4           | 1         |
| Example       | 8              | 2           | 1         |

- Number of bytes: 4

---

### Message Type 7 -- Display Update

#### Message Summary

- **Direction:** Received / Acted on
- **Purpose:** Updates OLED display with system status or hazard information.

| Field         | Byte 1–2       | Byte 3         | Byte 4–8     |
| ------------- | -------------- | -------------- | ------------ |
| Variable Name | `message_type` | `display_mode` | `data`       |
| Variable Type | `uint16_t`     | `uint8_t`      | `uint8_t[5]` |
| Min Value     | 7              | 0              | 0            |
| Max Value     | 7              | 5              | 255          |
| Example       | 7              | 1              | 10           |

- Number of bytes: 8

---

### Message Type 9 -- Status Request

#### Message Summary

- **Direction:** Received
- **Purpose:** Requests subsystem status information.

| Field         | Byte 1–2       |
| ------------- | -------------- |
| Variable Name | `message_type` |
| Variable Type | `uint16_t`     |
| Min Value     | 9              |
| Max Value     | 9              |
| Example       | 9              |

- Number of bytes: 2

---

### Message Type 10 -- Status Response

#### Message Summary

- **Direction:** Sent
- **Purpose:** Returns subsystem status and health information.

| Field         | Byte 1–2       | Byte 3        | Byte 4       |
| ------------- | -------------- | ------------- | ------------ |
| Variable Name | `message_type` | `status_code` | `error_flag` |
| Variable Type | `uint16_t`     | `uint8_t`     | `uint8_t`    |
| Min Value     | 10             | 0             | 0            |
| Max Value     | 10             | 5             | 1            |
| Example       | 10             | 1             | 0            |

- Number of bytes: 4

---

### Message Type 12 -- Emergency Stop

#### Message Summary

- **Direction:** Received / Acted on
- **Purpose:** Immediately halts subsystem operation for safety.

| Field         | Byte 1–2       |
| ------------- | -------------- |
| Variable Name | `message_type` |
| Variable Type | `uint16_t`     |
| Min Value     | 12             |
| Max Value     | 12             |
| Example       | 12             |

- Number of bytes: 2

---

## Coordination Notes

All message definitions must remain consistent across:

- Individual subsystem implementations
- Teammate subsystems
- Team documentation

---

## Deliverables

- API webpage
- PDF export of API page
- Packaged MPLAB project

---

## Submission Links

- API Page: [Add link]
- API PDF: [Add link]
- Software ZIP: [Add link]
