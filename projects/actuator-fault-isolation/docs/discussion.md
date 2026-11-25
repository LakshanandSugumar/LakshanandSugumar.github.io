# Discussion

!!! info "Web version"
    This page is a web rendering of the paper draft. Citations, equation numbers and final formatting are in the [PDF](https://github.com/LakshanandSugumar/actuator-fdi-paper/blob/main/main.pdf).

## Discussion and Limitations

- *Low speed.* The encoder-fault signature scales with speed, so a frozen encoder is invisible to $r_v$ below about 0.5 rad s<sup>−1</sup> at the joint for these parameters. High-frequency injection could cover standstill at the cost of noise and losses.

- *Model error.* Dead-time distortion, $\lambda_m$ error, and thermal drift of $R$ set $\tau_v$; the thresholds tolerated about 1.5 times the assumed error bounds. Online $R$ estimation would lower $c_2$.

- *Collision reaction.* The impact peak is set by the reflected inertia, so detection speed mainly prevents clamping rather than the first impact peak. The reflex also releases a payload for $T_\mathrm{coll}$ before it is recognized as Load; for a leg in stance this would need a gentler first reaction (e.g. admittance) instead of zero torque.

- *Friction.* Friction model error appears directly in $r_m$ and sets the smallest detectable contact torque; a learned friction and cogging model would tighten this threshold.

- *Reaction to encoder faults.* This paper stops at isolation and a safe state (ASC). Keeping the joint alive through sensorless control after an Enc alarm is ongoing work.

- *Simulation fidelity.* The plant ignores cogging, magnetic saturation, gearbox backlash, and compliance, which hardware will add.

## Conclusion

Two residuals already computable inside a PMSM joint actuator’s FOC loop, a back-EMF voltage residual and a generalized-momentum observer, have complementary sensitivity that isolates encoder faults from mechanical events without extra sensors. In simulation with up to 30 % model error, the scheme isolated 299 of 300 injected faults, raised no false alarms in an hour of normal motion, detected encoder faults within 0.51 ms, and detected collisions about three times faster than a following-error check.  Future work extends the scheme to encoder-fault-tolerant control so that a humanoid joint can keep operating after a sensor fault.

[^1]: 

## Hardware validation

The hardware section of the paper (testbed description, fault-injection protocol, measured latency, confusion matrix and ROC on real data) is being written as the [actuator testbed](https://lakshanandsugumar.github.io/projects/actuator-foc-testbed/) comes up on the bench. It will be added here with measured numbers only.

## Reproduce the simulations

```bash
git clone https://github.com/LakshanandSugumar/actuator-fdi-paper
cd actuator-fdi-paper/sim
pip install numpy numba matplotlib
python run_experiments.py   # about 70 s; regenerates figures/ and results/
```

All numbers quoted in the paper are in [`sim/results/results.json`](https://github.com/LakshanandSugumar/actuator-fdi-paper/blob/main/sim/results/results.json).

## Build the paper

```bash
pdflatex main && bibtex main && pdflatex main && pdflatex main
```
