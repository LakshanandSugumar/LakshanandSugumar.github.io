# API

## Overview

This page documents the messages used by my subsystem in the team UART daisy-chain protocol.

My board is the **Sensor + HMI subsystem**. It uses:

- BNO055 IMU
- HDC2080 temperature/humidity sensor
- OLED display
- pushbuttons
- LEDs
- UART communication with the rest of the team

This page lists the messages I send, receive, or act on, and expands each message into a complete specification.

---

## Subsystem Responsibilities

### Messages I send

- IMU / motion data
- temperature and humidity data
- local button events
- hazard or status updates
- acknowledgements when required by the protocol

### Messages I receive

- display update requests
- status requests
- motor or actuator status messages from the rest of the team
- any broadcast messages that my subsystem must act on

### Messages I act on

- emergency stop
- broadcast status / alert messages
- team-wide coordination messages

---

## Message Handling Rules

My receiver will:

- ignore characters outside of a valid message frame
- ignore malformed messages
- ignore messages larger than the allowed buffer
- pass through messages not addressed to me
- drop messages that originated from me
- process messages intended for my board
- provide a unique acknowledgement for each valid message received

My sender will:

- send properly formatted messages
- use valid prefix and suffix bytes
- keep message length within the protocol limit
- avoid putting prefix or suffix values into the message data
- use time-varying example values
- prioritize forwarding received messages before sending new ones
- limit sending rate with non-blocking code

---

## Message Type ** -- ********\_\_\_\_**********

### Message Summary

- **Direction:** Sent / Received / Broadcast / Acted on
- **Purpose:** ******************\_\_******************

### Message Table

| Field         |         Byte 1 |    Byte 2 |    Byte 3 |    Byte 4 |    Byte 5 |    Byte 6 |    Byte 7 |    Byte 8 |
| ------------- | -------------: | --------: | --------: | --------: | --------: | --------: | --------: | --------: |
| Variable Name | `message_type` | `field_2` | `field_3` | `field_4` | `field_5` | `field_6` | `field_7` | `field_8` |
| Variable Type |      `uint8_t` | `uint8_t` | `uint8_t` | `uint8_t` | `uint8_t` | `uint8_t` | `uint8_t` | `uint8_t` |
| Min Value     |            `0` |       `0` |       `0` |       `0` |       `0` |       `0` |       `0` |       `0` |
| Max Value     |          `255` |     `255` |     `255` |     `255` |     `255` |     `255` |     `255` |     `255` |
| Example       |            `1` |       `0` |       `0` |       `0` |       `0` |       `0` |       `0` |       `0` |

### Notes

- Number of bytes: \_\_\_
- Data type(s): \_\_\_
- Variable name(s): \_\_\_
- Smallest valid value recognized in code: \_\_\_
- Largest valid value recognized in code: \_\_\_
- Example valid payload: \_\_\_

---

## Message Type ** -- ********\_\_\_\_**********

### Message Summary

- **Direction:** Sent / Received / Broadcast / Acted on
- **Purpose:** ******************\_\_******************

### Message Table

| Field         |         Byte 1 |    Byte 2 |    Byte 3 |    Byte 4 |    Byte 5 |    Byte 6 |
| ------------- | -------------: | --------: | --------: | --------: | --------: | --------: |
| Variable Name | `message_type` | `field_2` | `field_3` | `field_4` | `field_5` | `field_6` |
| Variable Type |      `uint8_t` | `uint8_t` | `uint8_t` | `uint8_t` | `uint8_t` | `uint8_t` |
| Min Value     |            `0` |       `0` |       `0` |       `0` |       `0` |       `0` |
| Max Value     |          `255` |     `255` |     `255` |     `255` |     `255` |     `255` |
| Example       |            `2` |       `0` |       `0` |       `0` |       `0` |       `0` |

### Notes

- Number of bytes: \_\_\_
- Data type(s): \_\_\_
- Variable name(s): \_\_\_
- Smallest valid value recognized in code: \_\_\_
- Largest valid value recognized in code: \_\_\_
- Example valid payload: \_\_\_

---

## Example Message Specification Format

This is the style I will use for every message in my API.

### Message Type 64 -- Motor Speed Setpoint

| Field         |         Byte 1 |     Byte 2 |        Byte 3 |
| ------------- | -------------: | ---------: | ------------: |
| Variable Name | `message_type` | `motor_id` | `motor_speed` |
| Variable Type |      `uint8_t` |  `uint8_t` |      `int8_t` |
| Min Value     |           `64` |        `1` |        `-100` |
| Max Value     |           `64` |        `5` |         `100` |
| Example       |           `64` |        `3` |         `-30` |

---

## Receiver Behavior

My receiver must:

- handle all messages on the daisy-chain UART network
- pass on messages intended for other boards
- process messages intended for me
- ignore messages from myself
- ignore malformed or oversized messages
- acknowledge valid messages uniquely

---

## Sender Behavior

My sender must:

- send one example of each message type
- use valid message formatting
- include proper prefix and suffix
- stay within the allowed data length
- avoid invalid prefix/suffix bytes in message data
- use non-blocking timing for transmission
- be easy to modify for instructor testing

---

## Coordination Notes

If any message format changes, the team must update:

- my implementation
- my teammates’ implementations
- the team report webpage

---

## Deliverables

- API webpage
- API PDF export
- zipped software package

---

## Submission Links

- API Page: [link here]
- API PDF: [link here]
- Software ZIP: [link here]
