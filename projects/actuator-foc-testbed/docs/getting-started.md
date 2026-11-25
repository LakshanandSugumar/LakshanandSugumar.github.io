# Getting started

## 1. Install (Windows, macOS or Linux)

Python 3.10 or newer and a C compiler (for the firmware tests) are all you need.

```bash
git clone https://github.com/<you>/actuator-foc-testbed.git
cd actuator-foc-testbed
python -m venv .venv
.venv\Scripts\activate            # Windows   (macOS/Linux: source .venv/bin/activate)
pip install -e ".[dev]"
```

On Windows the firmware tests need `gcc` and `make`. MSYS2 (`pacman -S mingw-w64-ucrt-x86_64-gcc make`) or WSL both work.
STM32CubeIDE also ships a GCC, but it is the ARM cross-compiler and cannot build the host tests.

## 2. Check everything works (no hardware)

```bash
pytest -q                                   # 13 tests: sim, protocol, virtual joint over CAN
make -C firmware/tests                      # C unit tests + software-in-the-loop
cd sim
python -m experiments.run_all               # 8 figures + figures/results.json  (~10 s)
python -m experiments.make_video            # figures/actuator_sim_demo.mp4/.gif (~1 min, needs ffmpeg)
```

Expected output of the firmware tests:

```
SIL: velocity 30.017 rad/s, position 1.5712 rad (peak 1.5712), encoder fault 0x20 after 3.3 ms
all firmware core tests passed
```

## 3. Explore with the virtual joint

The virtual joint runs the simulation behind the real CAN protocol, so the host
tools behave exactly as they will with hardware:

```bash
python host/scripts/run_test.py --interface virtual --test velocity_step
python host/scripts/run_test.py --interface virtual --test position_step
python host/scripts/run_test.py --interface virtual --test sine_tracking
python host/scripts/run_test.py --interface virtual --test comm_timeout
```

Each run writes `logs/<time>_<test>.csv`, `.png` and `.json` (metrics).

To run the joint as a separate process, like a real device:

```bash
pip install msgpack                                   # needed by python-can's udp_multicast bus
python -m actuator_host.virtual_joint                 # terminal 1
python host/scripts/run_test.py --interface udp_multicast --channel 239.0.0.1 --test velocity_step   # terminal 2
```

Fault injection on the virtual joint: `--encoder-freeze-at 2.0` (seconds) or `--wall-at 0.6` (radians).

## 4. Change the design

| You want to | Edit | Then |
| --- | --- | --- |
| Use measured motor values | `MotorParams` in `sim/actuator_sim/params.py` and `act_default_config` in `firmware/core/src/actuator.c` | rerun sim + firmware tests |
| Change loop bandwidths | `Bandwidths` (sim) and `bw_*` (firmware); gains re-derive automatically | check `07_open_loop_bode.png` margins |
| Tune fault thresholds | `FaultThresholds` (sim) and the `*_trip`, `bemf_*`, `collision_nm` fields (firmware) | check no false trips: `pytest -q` |
| Add a CAN message | `firmware/core/inc/can_protocol.h` + `.c`, `host/actuator_host/protocol.py`, `docs/can_protocol.md` | add a byte-layout test on both sides |

## 5. Hardware wiring (NUCLEO-G474RE + X-NUCLEO-IHM07M1)

| Connection | From | To |
| --- | --- | --- |
| Driver | IHM07M1 stacked on the Nucleo Arduino/Morpho headers | jumpers per IHM07M1 manual for 3-shunt sensing |
| Motor phases | GM4108 three wires | IHM07M1 OUT1/2/3 (order only sets direction; calibration detects it) |
| DC bus | Bench supply 24 V, current limit 1 A to start | inline fuse, then e-stop (normally closed), then IHM07M1 VIN |
| Encoder | AS5047P breakout, 3.3 V | SPI1: SCK PA5, MISO PA6, MOSI PA7, CS any free GPIO named ENC_CS |
| Magnet | diametric 6 mm x 2.5 mm | centred on the motor shaft, 1-2 mm air gap to the chip |
| CAN | SN65HVD230 TX/RX | FDCAN1 PA12 (TX) / PA11 (RX); CANH/CANL to the CANable, 120 Ω at both ends |
| Thermistor | 10 k NTC on the stator + 10 k to 3.3 V | spare ADC input |

Check pins against the current Nucleo and IHM07M1 user manuals before powering up; CubeMX may assign alternatives.

## 6. Bring-up checklist

Follow it in order, and do not raise the supply current limit until a step passes.

1. **Power only, PWM off.** 12 V, 0.3 A limit. Board idles, STATUS frames arrive at 100 Hz (`run_test.py` prints an error if they do not).
2. **Sensors.** Turn the rotor by hand: position telemetry follows, one turn = 6.283 rad. Phase currents read about 0 A.
3. **PWM.** Scope the three phases at 50 % duty with no motor: 20 kHz, centre-aligned, and the ADC trigger sits in the middle of the low-side-on interval.
4. **Encoder calibration.** `python host/scripts/calibrate_encoder.py --interface gs_usb --channel 0 --write`. Travel must match the pole count, and the offset spread should stay under about 10° electrical.
5. **Current loop.** Clamp the rotor, use `MODE_CURRENT`, step 0.3 A. Compare with `sim/figures/01_current_step.png`.
6. **Velocity loop.** 5 rad/s, then 30 rad/s. `compare_sim.py` overlays sim and hardware.
7. **Position loop and tracking.** Steps, then sine with feedforward.
8. **Faults.** Kill the Python script (comms timeout), grab the inertia disk (collision), unplug the encoder while it turns slowly (encoder fault).

## 7. Troubleshooting

| Symptom | Likely cause |
| --- | --- |
| Motor vibrates or locks up in velocity mode | wrong encoder offset or direction: rerun calibration |
| ENC_MODEL fault as soon as the motor moves | wrong R, L or Kt, wrong pole count, or wrong current-sense gain; identify parameters (`system_identification.md`) |
| COLLISION trips on fast moves | J or friction values off, or threshold too low; check the `tau_ext` column in the log |
| No STATUS frames | bitrate mismatch, missing 120 Ω termination, wrong node id, or the CANable is in slcan mode while gs_usb is selected |
| Currents read a constant offset | offset calibration ran with PWM off or the motor moving; restart with the rotor still |
| COMM_TIMEOUT during long tests | PC sleeping or the USB adapter dropped; heartbeat runs at 50 Hz, timeout is 50 ms |
