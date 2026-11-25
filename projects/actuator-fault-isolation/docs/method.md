# Method

!!! info "Web version"
    This page is a web rendering of the paper draft. Citations, equation numbers and final formatting are in the [PDF](https://github.com/LakshanandSugumar/actuator-fdi-paper/blob/main/main.pdf).

## Introduction

Legged and humanoid robots are built from compact, highly geared or quasi-direct-drive joint actuators that run FOC at tens of kilohertz. Each actuator is a single point of failure: if the knee of a humanoid stops producing torque, the robot falls. At the same time, these actuators interact physically with people and the environment, so contacts must be detected fast enough to bound the contact force.

Most drives handle both problems with a small set of generic checks: hardware encoder error flags, over-current trips, and a following-error limit on $|q_\mathrm{ref}-\hat q|$. These checks have two weaknesses. First, they are slow or blind for an important class of faults: an encoder that *freezes* or *slips* (for example, a shifted magnet, a stuck SPI read, or a corrupted but CRC-valid frame) raises no flag at all. Second, they do not *isolate*: a large following error can be caused by a collision, by a heavy payload, or by a wrong encoder angle that misaligns the current vector. These events call for opposite reactions. A collision calls for compliance, a payload change calls for compensating the load, and an encoder fault calls for leaving angle-dependent control, because any reaction that trusts the encoder will make things worse.

This paper asks whether a joint actuator can tell a sensor fault from a mechanical event using only signals the FOC loop already has, without additional sensors. Our answer is a pair of residuals with complementary sensitivity:

- a *back-EMF voltage residual* $r_v$ that compares the commanded $dq$ voltages with those predicted by the electrical model at the encoder-reported speed and angle. Load torque changes the current, but the model accounts for the current, so $r_v$ is insensitive to load; an encoder error changes the predicted back-EMF, so $r_v$ is sensitive to encoder faults.

- a *generalized-momentum observer* $r_m$ that estimates the external joint torque. It responds to collisions and payload changes. It also reacts to encoder faults, because it uses the encoder speed, so its output is interpreted only while $r_v$ is quiet.

**Contributions.** (i) A structured residual design for PMSM joint actuators that decouples encoder faults from external torque using only FOC signals (4); (ii) a decision logic, including an angle-free voltage-pulse test, that isolates four fault classes and maps each to a reaction (5); (iii) a simulation study with model error, Monte Carlo fault injection, long normal runs, and ROC analysis against a following-error baseline (6); and (iv) a hardware validation on  with injected encoder faults, phase open circuits, and collisions at several speeds (7).

## Related Work

**Model-based FDI.** Residual generation with structured sensitivity is a classical tool for isolation. Our contribution is not a new observer class but a residual pair whose structural decoupling follows from the physics of the PMSM and joint, chosen so that it fits in the existing FOC interrupt.

**Collision detection in robots.** The generalized-momentum observer of De Luca *et al.* is the standard sensorless approach for manipulators and is surveyed in. These works assume healthy joint position sensing; we add a residual that certifies this assumption at run time.

**Position sensor faults in PMSM drives.** Sensor fault detection and tolerant control for electric drives has been studied mostly for traction and industrial drives, often by comparing the encoder with a sensorless estimate based on back-EMF. Open-phase faults and fault-tolerant topologies are reviewed in. 

## System Model

## Electrical model

In the rotor reference frame, a surface PMSM obeys $$\begin{aligned}
  v_d &= R\,i_d + L_d\,\dot i_d - \omega_e L_q\, i_q, \label{eq:vd}\\
  v_q &= R\,i_q + L_q\,\dot i_q + \omega_e\,(L_d\, i_d + \lambda_m), \label{eq:vq}
\end{aligned}$$ where $R$ is the phase resistance, $L_{d,q}$ the inductances, $\lambda_m$ the permanent-magnet flux linkage, and $\omega_e = p\,\dot\theta_m$ the electrical speed for $p$ pole pairs. The FOC loop computes the $dq$ quantities with the *measured* electrical angle $\hat\theta_e = p\,\hat\theta_m$. We write $\delta = \theta_e - \hat\theta_e$ for the angle error and $\hat\omega_e$ for the speed derived from the encoder.

## Mechanical model

With gear ratio $N$, torque constant $k_t = \tfrac{3}{2}p\lambda_m$, and reflected inertia $J$, the joint obeys $$J\,\ddot q = \tau_m - \tau_f(\dot q) - \tau_g(q) + \tau_\mathrm{ext},
  \qquad \tau_m = N k_t\, i_q, \label{eq:mech}$$ where $\tau_f$ is friction, $\tau_g$ a known gravity or spring term (zero for a horizontal joint), and $\tau_\mathrm{ext}$ the unknown external torque from contact or payload.

## Fault classes

We consider four classes, summarized in 1: Enc (encoder freeze, offset/slip, or disconnect), Phase (open circuit of one motor phase), Coll (transient external torque from contact), and Load (persistent change in external torque, e.g. a payload).

## Residual Design

## Back-EMF voltage residual

Let $v^*_{dq}$ be the voltages commanded by the current controllers (available in the FOC loop before modulation). The model-predicted voltage at the encoder speed is $$\hat v_{dq} = R\,i_{dq} + L\,\dot i_{dq}
   + \hat\omega_e \begin{bmatrix} -L_q i_q \\ L_d i_d + \lambda_m \end{bmatrix},$$ and the residual is $r_v = v^*_{dq} - \hat v_{dq}$. Neglecting inverter nonlinearity and parameter error for the moment, and expressing the true back-EMF in the frame defined by $\hat\theta_e$, gives $$r_v \approx \lambda_m
  \begin{bmatrix} -\omega_e \sin\delta \\ \omega_e\cos\delta - \hat\omega_e \end{bmatrix}.
  \label{eq:rv}$$ Two properties follow directly from [eq:rv]:

1.  *Load insensitivity.* $\tau_\mathrm{ext}$ enters only through $i_{dq}$, which is measured and compensated in $\hat v_{dq}$. Hence $r_v \approx 0$ for any load when $\delta = 0$ and $\hat\omega_e = \omega_e$.

2.  *Encoder sensitivity.* An angle offset ($\delta\ne0$) gives $\|r_v\|\approx 2\lambda_m|\omega_e\sin(\delta/2)|$, and a frozen encoder ($\hat\omega_e \to 0$, $\delta$ growing) gives $\|r_v\|\approx\lambda_m|\omega_e|$.

To avoid differentiating noisy currents, $r_v$ is computed after a first-order low-pass filter $F(s)=a/(s+a)$ applied to both $v^*$ and the model, with the $L\,\dot i$ term computed as $L\,a\,(i - F i)$. Unmodeled terms, mainly dead-time voltage error, thermal drift of $R$, and error in $\lambda_m$, are bounded by a threshold that follows their structure: $$\|r_v\| > \tau_v = c_0 + c_1|\hat\omega_e| + c_2\|i_{dq}\|. \label{eq:thr_v}$$ We set $c_1 = 1.5\,\Delta\lambda_m$ and $c_2 = 1.5\,\Delta R$ from the assumed parameter-error bounds, and $c_0$ to 1.25 times the largest remaining residual over a calibration set of normal motion (6). Because the fault signature scales with $|\omega_e|$, a frozen encoder is detectable only above $|\omega_e| > c_0/(\lambda_m - c_1)$. A frozen reading at standstill is correct until the joint moves, so this limit mostly delays detection until motion starts (6).

## Generalized-momentum observer

Following, define the momentum $\rho = J\dot q$ and the residual $$r_m(t) = K\!\left[\rho(t) - \!\int_0^t\!\big(\tau_m - \hat\tau_f(\dot q)
   - \tau_g(q) + r_m\big)\,ds - \rho(0)\right], \label{eq:mo}$$ with gain $K>0$. Substituting [eq:mech] gives $\dot r_m = K(\tau_\mathrm{ext} - r_m)$, so $r_m$ is a first-order filtered estimate of $\tau_\mathrm{ext}$ (plus friction and inertia model error) with bandwidth $K$, and needs no acceleration measurement. An alarm is raised when $|r_m| > \tau_m^\mathrm{thr}$.

The observer uses $\dot q$ from the encoder, so an encoder fault corrupts $r_m$: a freeze makes $\rho$ drop to zero and $r_m$ shows a spurious torque. In simulation $r_m$ often alarms *before* $r_v$ on an encoder freeze (2). This is why $r_m$ alone cannot isolate, and why the logic below waits briefly for $r_v$ before acting on a mechanical label.

## Discrete implementation

Both residuals run at the current-loop rate $f_s=20$ and need roughly 30 multiply-accumulates per cycle on top of FOC (our estimate). Each alarm requires $n_d = 5$ consecutive samples (0.25 ms) over threshold. 

## Fault Isolation and Reaction

is the structure (incidence) matrix of the residual set, extended with the reaction for each class.

<div markdown id="tab:faults">

| Class                                | $r_v$ |   $r_m$    |   pulse test   | Reaction             |
|:-------------------------------------|:-----:|:----------:|:--------------:|:---------------------|
| Enc   |       |   $\ast$   |    balanced    | ASC                  |
| Phase |       |   $\ast$   | one phase dead | ASC                  |
| Coll  |   0   | transient  |       –        | zero torque          |
| Load  |   0   | persistent |       –        | resume + feedforward |

Fault signatures and reactions.: residual fires; 0: residual stays below threshold; $\ast$: may fire but is not trusted.

</div>

The logic is evaluated every cycle:

1.  *$r_v$ alarm.* The encoder or the electrical path is suspect, so $r_m$ is disregarded and the inverter enters active short circuit (ASC), a standard safe state for PMSM drives that needs no rotor angle. After 2 ms of settling, three 0.1 ms, 2 V voltage pulses are applied along the $a$, $b$, and $c$ phase axes in the stationary frame, again without the rotor angle. If one phase’s current response is below 30 % of the median response, the class is Phase; otherwise it is Enc.

2.  *$r_m$ alarm, $r_v$ quiet.* The event is treated as a possible collision at once: the torque command is set to zero (compliant). If $r_v$ alarms within the next 40 ms, step 1 takes over. After $T_\mathrm{coll}=40$ the class is Load if $|r_m|$ is still above threshold, and position control resumes with $-r_m$ as torque feedforward; otherwise the class is Coll and the joint stays compliant.

3.  Otherwise the actuator is healthy.

Reacting on the first $r_m$ alarm keeps the collision reaction latency equal to the detection latency, at the cost of briefly releasing a payload before it is recognized as Load.

<figure markdown id="fig:arch">

<figcaption>FDI architecture. Both residuals use only signals already present in the FOC loop; no sensors are added.</figcaption>
</figure>