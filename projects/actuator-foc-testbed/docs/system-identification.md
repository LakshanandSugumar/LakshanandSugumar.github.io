# System identification: replacing the placeholder motor numbers

Every gain is derived from `MotorParams` (sim) / `act_default_config` (firmware),
so identify these once on the bench and paste them into both places.

| Parameter | Method | Tool |
| --- | --- | --- |
| Pole pairs p | Spin by hand, count cogging steps / 2, or the calibration script travel check | `calibrate_encoder.py` |
| Phase resistance R | DC current through two phases with the rotor locked: R_phase = V / I / 2. Use 4-wire measurement or the bench supply at low current | multimeter |
| Inductance L | Locked rotor, MODE_CURRENT step of 0.3 A with a low-gain loop, fit the first-order rise (tau = L/R); or an LCR meter between two phases / 2 | `run_test.py` + fit |
| Electrical offset, direction | Open-loop alignment at several electrical angles | `calibrate_encoder.py --write` |
| Kt | Lever arm on a scale (or load cell): hold MODE_TORQUE at several iq, Kt = F x r / iq. Cross-check with back-EMF: spin with another motor or by hand, Kt = 1.5 p lambda | scale, `run_test.py` |
| J, b, Tc | Torque chirp (`--test chirp_torque`) and constant-velocity runs: Tc + b w = Kt iq at steady speed (fit a line over 5-50 rad/s); J from the chirp frequency response or from accel/decel slopes | `chirp_torque` log + least squares |
| Thermal R_th, C_th | Hold 1.5 A locked for 10 min with the NTC logged, fit a first-order rise | NTC channel |

After updating the numbers: rerun `python -m experiments.run_all` and `make -C firmware/tests`,
then compare the hardware step logs against the new sim figures. The size of the gap is a result
worth reporting (model fidelity).
