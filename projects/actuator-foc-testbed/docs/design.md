# Design and theory

This document explains *why* the controller looks the way it does. Every number
here comes from `sim/actuator_sim/params.py` and is reproduced by
`python -m experiments.run_all`.

## 1. Plant model

A surface-mounted PMSM (gimbal and most outrunner robot motors) has equal d and q
inductance, so in the rotor frame:

```latex
L \frac{di_d}{dt} = v_d - R i_d + \omega_e L i_q
L \frac{di_q}{dt} = v_q - R i_q - \omega_e L i_d - \omega_e \lambda
\tau_m = \tfrac{3}{2} p \lambda i_q = K_t i_q
J \dot\omega = \tau_m - b\omega - T_c \,\mathrm{sgn}(\omega) + \tau_{ext}
```

with p pole pairs, electrical speed ω_e = p·ω and flux linkage λ = K_t / (1.5 p).
Torque depends only on i_q, so FOC holds i_d = 0 and controls torque through i_q.

The simulation integrates this at 5 substeps per 50 µs PWM period. Voltages are
applied in the stationary (α, β) frame and held over each period (average-value
inverter), so rotor motion inside a period and any controller angle error are
captured physically, not approximated. The controller output computed at sample k
is applied during period k+1 (one-period compute delay), as on the STM32.

Sensor models: 14-bit absolute encoder (AS5047P), phase current noise of 10 mA rms
plus 12-bit ADC quantisation.

## 2. Current loop (20 kHz)

The plant from v_q to i_q (after decoupling) is first order:

```latex
G_i(s) = \frac{1}{Ls + R}
```

A PI with Kp = L·ω_c and Ki = R·ω_c puts its zero on the plant pole, so the loop
gain is ω_c / s and the closed loop is first order with bandwidth ω_c. The design
uses f_c = 1 kHz. The 1.5-sample delay (compute + PWM) costs ω_c · 1.5 T_s = 27°,
leaving a 63° phase margin (`07_open_loop_bode.png`). 1.5 kHz was tried first and
gave 49° margin and 19 % overshoot, so it was lowered.

Additions on top of the PI:

- **Decoupling and back-EMF feedforward**: v_d gets -ω_e L i_q, v_q gets ω_e L i_d + ω_e λ, so the PI only fights model error.
- **Angle advance**: the inverse Park uses θ_e + 1.5 T_s ω_e, so the voltage lands where the rotor will be when the PWM applies it.
- **Circular voltage limit with d priority**: |v| ≤ V_bus/√3 (the linear SVPWM range); q gets what d leaves.
- **Conditional-integration anti-windup**: the integrator stops when the output is saturated and the error pushes further into saturation.

Note on large steps: with Kp = 14 V/A, a 1 A step asks for more voltage than the
13.9 V the inverter can make, so the response becomes slew-limited by V_bus. The
step test uses 0.5 A to measure the linear loop.

## 3. Velocity loop (5 kHz) and the PLL

The encoder difference over one 200 µs velocity period is quantised to
2π/16384 / 200 µs ≈ 1.9 rad/s, which is too noisy for a fast loop. A type-2
phase-locked loop tracks the multi-turn angle and gives a smooth velocity:

```latex
e = \theta_{meas} - \hat\theta, \quad \dot{\hat\omega} = k_i e, \quad \dot{\hat\theta} = \hat\omega + k_p e
```

with k_p = 2ω_n, k_i = ω_n² (ζ = 1) and f_n = 400 Hz.

With the current loop much faster, the velocity plant is K_t / (J s). A PI with
Kp = J ω_v / K_t crosses over at ω_v (f_v = 80 Hz) and the integral corner sits at
ω_v / 5, giving 68° phase margin. Torque feedforward enters as τ_ff / K_t.

## 4. Position loop (1 kHz)

Near the target the loop is a P controller with Kp = ω_p (12 Hz), giving a
velocity command. For a large step a linear P asks for full speed until the last
moment, and the current limit cannot brake in time, which caused 11-13 % overshoot.
The fix is a braking curve: the velocity command is the smaller of the linear law
and the speed from which the joint can still stop,

```latex
v_{cmd} = \mathrm{sgn}(e)\,\min\left(K_p |e|,\ \sqrt{2 a_{max} |e|}\right), \quad a_{max} = 0.7\,K_t I_{max} / J
```

which removes the overshoot (90° step: 0 %, 67 ms settle). For trajectories the host
also sends velocity and torque feedforward (J q̈ + b q̇ + T_c sgn q̇). That takes
2 Hz sine tracking error from 6.6° to 0.037° RMS.

## 5. Fault detection

| Detector | Signal | Catches | Ignores |
| --- | --- | --- | --- |
| Over-current | max phase current > 4 A | shorts, runaway | - |
| Encoder flag / jump | AS5047 parity, error bit, Δθ > 0.05 rad per tick | wiring and SPI faults | silent freezes |
| **Back-EMF residual** | applied voltage minus R i + L di/dt + ω_e L J i + ω_e λ | silent encoder freeze, magnet slip, wrong offset | load torque, collisions |
| **Momentum observer** | estimated external torque | collisions, jams, unexpected loads | sensor faults (while the encoder is healthy) |
| Following error | position error > 0.35 rad | gross failures | fast, small contacts |
| Thermal model | I²R with copper tempco into a first-order thermal mass | overheating | - |
| Comms watchdog | no host frame for 50 ms | crashed PC, unplugged CAN | - |

### Back-EMF (voltage-model) residual

The voltage applied over the last PWM period, rotated into the mid-period measured
frame, must equal what the model predicts from the measured currents and the
*measured* speed:

```latex
r = v_{applied} - \left(R\,\bar i + L\,\frac{\Delta i}{T_s} + \hat\omega_e L J \bar i + \hat\omega_e \lambda\,\hat q\right)
```

If the encoder freezes while the rotor turns, ω̂_e drops to zero but the real
back-EMF does not, so |r| jumps to about ω_e λ. External torque changes the current
but not this voltage balance, so the residual separates sensor faults from
mechanical events. Two implementation details mattered:

- Filtering |r| directly rectified current-sense noise into a 0.8 V bias. Filtering the *vector* (r_d, r_q) at 150 Hz first and then taking the magnitude dropped the normal level to about 0.03 V.
- The applied voltage must be expressed in the frame at the middle of the period it was applied in, not in the delay-compensated frame it was computed in.

Threshold 1.2 V held for 2 ms: the largest normal value seen in aggressive
current-limited moves is 0.87 V; a freeze at 20 rad/s reaches about 3 V. Detection
takes 3.3 ms in both the Python model and the C firmware.

### Generalised momentum observer

```latex
r(t) = K\left(J\hat\omega(t) - J\hat\omega(0) - \int_0^t \left(\tau_m - b\hat\omega - T_c\,\mathrm{sgn}\,\hat\omega + r\right) d\tau\right)
```

r follows the external torque through a first-order filter of bandwidth K (60 Hz)
without differentiating velocity. Because the PLL velocity lags the true velocity
by about k_p/k_i during acceleration, a current-limited velocity step produced a
0.09 Nm false reading. Lagging τ_m by 1.5 k_p/k_i (tuned in sim) cut that to
0.018 Nm, under the 0.08 Nm collision threshold. Result: contact is detected 0.8 ms
after it starts, and peak contact torque is 0.12 Nm, against 30 ms and 0.45 Nm for a
following-error check.

## 6. Simulation to firmware

`firmware/core/src/actuator.c` is a line-for-line port of
`sim/actuator_sim/control.py` + `faults.py`. The firmware test
(`make -C firmware/tests`) runs the C controller against a C copy of the plant and
must reproduce the sim: 30.02 rad/s on a 30 rad/s step, 1.5712 rad on a 90° step,
encoder freeze caught in 3.3 ms. When you change one side, change the other and run
both test suites.

## 7. Known limitations

- Motor parameters are estimates until bench identification.
- Single-precision multi-turn angle loses resolution after many turns (about 7 µrad at 100 rad). It is fine for a joint with limited travel. A continuous-rotation wheel would need an integer turn counter.
- No field weakening, cogging compensation or dead-time compensation yet. The L6230 handles dead time internally.
- Trajectories streamed from a PC show timing jitter. The over-CAN virtual test gave 3.6° RMS tracking error against 0.037° in pure sim. Next step: an on-board trajectory generator.
