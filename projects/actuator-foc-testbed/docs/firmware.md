# Firmware (STM32G474 + IHM07M1)

```
firmware/
  core/            hardware-independent C99: FOC math, cascade, faults, CAN protocol, AS5047
  app/main_stm32.c HAL glue: ISR wiring, CAN, startup calibration (paste into a CubeIDE project)
  tests/           host build of core/ + unit tests + software-in-the-loop run
```

`core/` is the same algorithm as `sim/actuator_sim/` (gains, anti-windup, PLL,
back-EMF residual, momentum observer). Run the host tests after every change:

```bash
make -C firmware/tests      # builds with -Werror -Wdouble-promotion, runs unit + SIL tests
```

The SIL test closes the loop between the real `actuator.c` and a C port of the
plant model and checks a velocity step, a 90 deg position step and silent
encoder-freeze detection.

## CubeMX setup (NUCLEO-G474RE)

| Peripheral | Setting |
| --- | --- |
| SYSCLK | 170 MHz (HSE 24 MHz from ST-Link MCO or HSI + PLL) |
| TIM1 | Center-aligned mode 1, ARR = 4250 (20 kHz at 170 MHz), CH1/2/3 PWM (IHM07M1 IN1/2/3), CH4 PWM at ARR-1 as TRGO2 for ADC, dead time not needed (L6230 handles it), break input to the e-stop line |
| ADC1 / ADC2 | Injected group triggered by TIM1 TRGO2; ADC1 rank1 = phase A shunt, rank2 = VBUS; ADC2 rank1 = phase B shunt; sample time ~ 24.5 cycles |
| SPI1 | Mode 1 (CPOL 0, CPHA 1), 16-bit, 5-10 MHz, software CS on a GPIO named ENC_CS |
| FDCAN1 | Classic CAN, 1 Mbit/s (prescaler/segments from the CubeMX calculator, sample point ~ 80 %), pins PA11/PA12 to SN65HVD230 |
| TIM6 | 1 kHz update interrupt |
| GPIO | DRV_EN (L6230 EN1/2/3 tied), ENC_CS, NTC on a spare ADC channel |
| NVIC priorities | ADC injected (0, highest) > FDCAN (2) > TIM6 (3) |

Check the IHM07M1 user manual for the exact shunt pins and op-amp gain; the
`ADC_TO_AMPS` constant in `main_stm32.c` is a starting value to calibrate
against a bench multimeter.

## Bring-up order (do not skip)

1. Bench supply at 12 V, 0.5 A limit. Run with `MODE_IDLE` and check telemetry: angle tracks hand rotation, currents read ~0.
2. Encoder direction and electrical offset: `host/scripts/calibrate_encoder.py` (holds a d-axis current vector, reads the angle, computes `enc_offset_e`, checks direction).
3. Current loop alone (`MODE_CURRENT`) with the rotor clamped. Compare the step to `sim/figures/01_current_step.png`.
4. Velocity loop, then position loop. Raise the supply limit only after each loop matches the sim.
5. Fault tests: unplug the encoder connector while spinning slowly, stop the host program (comms timeout), grab the inertia disk (collision).

## CPU budget (estimate, check with a GPIO toggle + scope)

At 170 MHz the 20 kHz ISR has 8500 cycles. Expected cost: ~300 cycles for the
current loop with `sinf/cosf` from newlib (replace with CMSIS-DSP
`arm_sin_cos_f32` or the CORDIC peripheral if needed), ~400 for the residual and
observer, ~600 for the SPI read at 10 MHz. Budget leaves >80 % headroom.
