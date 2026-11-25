# CAN protocol

Classic CAN, 1 Mbit/s, 8 data bytes, little-endian. 11-bit identifier:

```
ID = (msg << 4) | node_id        node_id 0..15, msg 0x00..0x7F
```

Source of truth: `firmware/core/inc/can_protocol.h` (C) and `host/actuator_host/protocol.py` (Python).
`host/tests/test_protocol.py` and `firmware/tests/test_main.c` check the same byte layout.

## Host to joint

| msg | Name | Payload | Notes |
| --- | --- | --- | --- |
| 0x01 | SET_MODE | u8 mode | 0 idle, 1 torque, 2 velocity, 3 position (holds current angle), 4 current |
| 0x02 | SET_TORQUE | f32 torque [Nm] | iq = tau / Kt, clamped to I_max |
| 0x03 | SET_VELOCITY | f32 vel [rad/s], f32 torque_ff [Nm] | |
| 0x04 | SET_POSITION | f32 pos [rad], i16 vel_ff [0.01 rad/s], i16 torque_ff [mNm] | stream at 200-1000 Hz for trajectories |
| 0x05 | SET_CURRENT | f32 iq [A], f32 id [A] | loop tests only |
| 0x06 | CLEAR_FAULTS | - | returns to IDLE |
| 0x07 | ESTOP | - | IDLE immediately |
| 0x08 | SET_PARAM | u8 param, 3 pad, f32 value | only in IDLE; re-derives all gains |
| 0x09 | HOST_HEARTBEAT | - | any host frame resets the 50 ms comms watchdog |
| 0x0A | CALIBRATE | f32 electrical angle [rad], f32 current [A] | open-loop alignment for encoder offset |

SET_PARAM ids: 1 current bandwidth [Hz], 2 velocity bandwidth, 3 position bandwidth,
4 collision threshold [Nm], 5 encoder electrical offset [rad], 6 current limit [A].

## Joint to host

| msg | Name | Rate | Payload |
| --- | --- | --- | --- |
| 0x10 | STATUS | 100 Hz | u8 mode, i8 winding temp estimate [C], u16 fault bits, u32 time [us] |
| 0x11 | TELEM_STATE | 1 kHz | f32 position [rad], f32 velocity [rad/s] |
| 0x12 | TELEM_CURRENT | 1 kHz | i16 iq [mA], i16 iq_ref [mA], i16 id [mA], u16 vbus [10 mV] |
| 0x13 | TELEM_OBS | 1 kHz | i16 observer external torque [mNm], u16 voltage residual [mV], u32 time [us] |

Bus load: 3 telemetry frames x 1 kHz x ~130 bits = ~39 % of 1 Mbit/s for one
joint, plus commands. For more joints, drop telemetry to 500 Hz or move to CAN-FD
(the G474 supports it; the CANable 2.0 does too with the right firmware).

## Fault bits

| Bit | Fault | Detector |
| --- | --- | --- |
| 0 | OVERCURRENT | any phase above 4 A (instantaneous) |
| 1 | OVERTEMP | lumped thermal model with copper tempco, 90 C |
| 2 | UNDERVOLT | bus below 18 V |
| 3 | ENC_FLAG | AS5047 parity or error flag |
| 4 | ENC_JUMP | angle change above 0.05 rad in one 50 us tick |
| 5 | ENC_MODEL | back-EMF voltage residual above 1.2 V for 2 ms (silent encoder fault) |
| 6 | FOLLOWING | position error above 0.35 rad |
| 7 | COLLISION | momentum-observer external torque above 0.08 Nm |
| 8 | COMM_TIMEOUT | no host frame for 50 ms while active |
