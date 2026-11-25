# Design notes

This document explains how each layer works and why it was built that way. The README covers results and usage.

## 1. Loop timing

The simulator runs at 10 Hz (`dt = 0.1 s`). Every tick:

1. **Perception** (`perception.py`) takes the true world state and returns what the ego can see. An agent is visible if any of its sample points (centre plus 4 corners, or the centre only for pedestrians) has a clear line of sight from the front bumper, within 150 m. It also checks 9 points on the right curb at each crosswalk ahead. If any of them is hidden, it records an `OccludedArea` with the id of the blocking object.
2. **Latency buffer**. The stack does not see the newest observation. It sees the one from `latency` seconds ago. The plan it produces is then tracked from that older time origin, so a 300 ms latency means the controller is always following a plan made from a 300 ms old world.
3. **Reasoning** (`reasoning.py`) turns the observation into a `Decision`, which holds a maneuver, `Constraints` and a list of `Rationale`. With the adaptive policy, this step is skipped on calm ticks and the cached decision is reused.
4. **Planning** (`planner.py`) samples trajectories inside the constraints, checks them against predictions and picks the cheapest. It runs every tick even when reasoning is skipped, which keeps collision checking always fresh.
5. **Control** (`control.py`) uses pure pursuit for steering and feed-forward plus P control for speed.
6. **Physics** (`world.py`) is a kinematic bicycle model about the centre of gravity, with limits on steering angle, steering rate and acceleration. All other agents follow their scripted behaviours.
7. **Metrics** run on the *ground truth*: collisions use the separating-axis theorem on the true polygons, and clearances use the true polygons too.

## 2. The reasoner: constraints, not trajectories

The reasoner never outputs a path. It outputs:

| field | meaning |
|---|---|
| `v_max` | speed cap (a target, and a hard limit after a short comfort ramp) |
| `stop_x` | the front bumper must stay behind this x |
| `y_lo, y_hi` | allowed lateral corridor for the ego centre |
| `y_pref` | preferred lateral position |
| `lead_id` | vehicle to keep a safe following gap to |

Each rule writes into these fields and adds a `Rationale(rule, maneuver, agent_ids, text)`. When rules are combined, the tightest value wins (minimum speed cap, earliest stop line, intersection of corridors), and the reported maneuver is the one with the highest priority:

`EMERGENCY > YIELD > WAIT > NUDGE = OVERTAKE > CAUTION > FOLLOW > CRUISE`

Keeping the reasoner's output small and structured is what makes the explanations testable. Each explanation names specific agents, and each constraint can be traced back to the rule that set it.

### Rules

| rule | trigger | effect |
|---|---|---|
| `lead` | same-direction vehicle in the corridor ahead | `v_max = v_lead + (gap - (s0 + T·v)) / 2`, plus `lead_id` |
| `cut_in` | adjacent vehicle whose lateral velocity carries it into the corridor within 2 s | same as `lead`, raised earlier |
| `crosswalk` | pedestrian on the road at a crosswalk who hasn't cleared our lane, or one stepping off the curb | `stop_x` = 1 m before the crosswalk (only if stopping needs ≤ 7 m/s²) |
| `curb` | pedestrian standing at the curb by a crosswalk | `v_max = max(6, v_stop(d))` |
| `occlusion` | crosswalk entry hidden by an object | `v_max = max(3, v_stop(d))`, citing the blocker |
| `oncoming_nudge` | oncoming vehicle whose near edge leaves less than 0.6 m of clearance | corridor shifted right onto the shoulder, `v_max = 7` within 50 m |
| `oncoming_block` | no room at all | pull right and stop |
| `overtake_sight` | parked obstacle in lane, but the oncoming lane can't be seen far enough (range limit or the obstacle itself) | stop 10 m behind, edge left to look past it |
| `overtake_wait` | parked obstacle, and an oncoming vehicle reaches the passing zone before `t_need + 2.5 s` | stop behind it |
| `overtake` | parked obstacle and the gap is clear | corridor widened into the oncoming lane, `y_pref` = oncoming lane centre until passed |

`v_stop(d)` is the highest speed from which the car can stop within `d` metres, allowing for a reaction time: it solves `v·t_r + v²/(2a) = d` with `t_r = 1 s` and `a = 3 m/s²`. Without the reaction-time term, the car always arrived about 0.8 s "late" to its own speed cap and needed emergency braking when the pedestrian appeared. Simulation runs caught this.

Overtaking keeps a little memory: once the ego has started to pull out, it stays committed, unless it hasn't yet left its own lane, in which case it can still abort.

## 3. The planner

The planner follows the Frenet sampling idea from Werling et al. (2010). On a straight road, `(s, d) = (x, y)`.

* **Longitudinal**: quartic velocity-keeping profiles to `v_max × {1, .75, .5, .25, 0}` over `T ∈ {1.5, 3, 5} s`. When a stop line is active, it adds quintic stop-at-position profiles and one constant-deceleration "brake to the line" profile.
* **Lateral**: quintic in *distance travelled*, `d(s)`, toward each of up to 6 target offsets (5 spread across the corridor plus `y_pref`), over lengths `max(v·T, L_min)`. The first version used `d(t)`. It asked for sideways motion that a car can't make at walking speed, and overtakes from a standstill failed because the bicycle model couldn't follow. Planning `d(s)` makes every candidate drivable by construction.
* **Feasibility checks** (vectorised over all candidates × all time steps):
  no reversing; `a ∈ [-6, 2.5]`; heading slope `|dy/dx| ≤ 0.7`; curvature `≤ min(κ_steer, a_lat,max / v²)`; stay inside the corridor or move steadily back into it; speed cap with a 0.8 s ramp; stop line, including the requirement that the car can still stop *after* the horizon ends (`x_end + v_end²/2a ≤ stop_x`); and collisions.
* **Collision model**: the ego is 4 circles along its heading, and every other agent is an oriented rectangle moving along its predicted path. The margins are 0.3 m laterally and 1.0 m longitudinally for vehicles, and 0.8 m for pedestrians. If the ego is *already* inside the margin, for example while squeezing past a bus, a candidate is rejected only if it gets closer.
* **Cost**: lateral and longitudinal jerk, lateral acceleration, distance from the target speed, distance from `y_pref`, longitudinal acceleration, an exponential proximity penalty and corridor violation.
* **Fallback**: if nothing is feasible, the planner brakes at -8 m/s² in a straight line. The reasons candidates were rejected become the emergency explanation, for example "every comfortable trajectory comes too close to S1".

## 4. Explanations and faithfulness

An explanation event is logged only when the pair (maneuver, primary rule, cited agents) changes and the new decision has lasted 3 ticks. Emergencies are logged immediately. This keeps explanations "when needed": a whole scenario produces 2–5 sentences, not 150.

`faithfulness.py` checks each event against counterfactual scenes, using the same reasoner state and the same observation:

* **necessity**: removing each *cited* agent must change the decision materially (the maneuver, or any constraint by more than 0.5 m/s, 0.2 m or 1 m);
* **completeness**: removing each *uncited* visible agent must *not* change it.

This test caught a real bug. In the stalled-car scenario, the wait message cited the oncoming car C1. But the ego could not have passed anyway, because it couldn't see far enough down the oncoming lane, so removing C1 changed nothing. The decision was overdetermined, and the explanation blamed the wrong thing. The fix was to cite only the binding reason (the limited view) and mention C1 in the text without citing it.

## 5. Adaptive ("fast/slow") reasoning

`scene_complexity()` is a cheap score computed every tick from the number, type and closing speed of nearby agents and from occlusions. The full reasoner runs when:

* the complexity is at least 3, or
* the complexity has changed by at least 1 since the last run, or
* 1 s has passed since the last run.

Otherwise the last decision is reused. The planner still runs every tick, so collision checking is never stale. The comparison experiment measures how many reasoner calls this saves and whether it costs anything in collisions, emergency braking or comfort.

## 6. LLM/VLM plug-in

`llm_reasoner.GuardedLLMReasoner` writes the observation out as text and asks any `prompt -> str` backend for JSON. It then merges that answer with the rule-based decision so the **more cautious** value wins on every field. The model can add caution (a lower speed or an earlier stop) and choose a position inside the rule-based corridor. It can never widen the envelope. If the output can't be parsed, the reasoner falls back to the rules alone. This is the slot for comparing an open model against the rules under identical scenarios.

## 7. Known simplifications

* The road is straight and there are no junctions. Curved roads would need a proper Frenet transform.
* Perception is perfect inside line of sight: there is no noise, no false positives and no tracking.
* Other agents follow scripts and don't react to the ego. Because of this, the reactive-baseline "bus collisions" partly come from a bus driver who never moves over.
* Prediction is constant velocity. The reactive baseline ignores lateral velocity (it assumes lane keeping).
* Planner runtimes are for Python and NumPy on a laptop-class CPU. They show relative cost, not embedded performance.
