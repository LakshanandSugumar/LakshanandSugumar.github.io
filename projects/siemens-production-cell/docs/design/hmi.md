# WinCC HMI specification

Target: WinCC Unified PC runtime (or a Comfort panel such as TP1200 Comfort if Unified is not in the license), simulated alongside PLCSIM Advanced.

## Rules for every screen

- One colour rule: grey idle, green running, yellow waiting / manual, red fault. Colour is never the only signal; every state also has a text label.
- A fixed header on all screens: cell state, mode, active alarm count, date/time. Navigation buttons along the bottom.
- Commands are pulses written to `DB_HMI` (`Cmd_Start`, `Cmd_Stop`, `Cmd_Reset`); the PLC resets them. The HMI never writes process outputs directly.

## Screens

| # | Screen | Content | Tags |
| --- | --- | --- | --- |
| 1 | Cell overview | Line mimic: three stations, conveyors and reject lane, each station coloured by its state and showing its active step number and name. Mode selector, Start / Stop / Reset. Good, reject, total counts. | `DB_Production.*`, `DB_HMI.*` |
| 2 | Station detail (x3) | GRAPH overview control bound to the station's GRAPH instance DB: active step, step time, interlock and supervision status. Text of the current step. | `GR_StationN` instance DB |
| 3 | Manual / Setup | One row per actuator: jog button, current position feedback, and the interlock condition as a lamp + text ("Blocked: Z not at top"). Visible and enabled only in Manual / Setup. | process tags, interlock bits from `FB_StationN` |
| 4 | Alarms | Alarm view (active + history), acknowledge button, filter by station. | system alarms |
| 5 | Production | OEE and its three factors since counter reset, cycle time vs ideal, counter reset button (with confirmation). | `DB_Production.*` |

## Sketch, screen 1

```
┌──────────────────────────────────────────────────────────────────────┐
│ RUNNING · AUTO                      Alarms: 0          2026-12-20 14:02│
├──────────────────────────────────────────────────────────────────────┤
│  ┌─────────────┐      ┌──────────────────┐      ┌─────────────┐        │
│  │ 1 Infeed    │ ───► │ 2 Pick & place    │ ───► │ 3 Outfeed   │        │
│  │ S2 Convey   │      │ S5 Grip lid       │      │ S1 Wait     │        │
│  └──────┬──────┘      └──────────────────┘      └─────────────┘        │
│         └─► reject lane                                                 │
│                                                                         │
│   Good 1 204    Reject 61    OEE 78.4 %       [AUTO|MANUAL|SETUP]       │
│                                              [START] [STOP] [RESET]     │
├──────────────────────────────────────────────────────────────────────┤
│ [Overview] [Station 1] [Station 2] [Station 3] [Manual] [Alarms] [OEE] │
└──────────────────────────────────────────────────────────────────────┘
```

Export screenshots of each finished screen to `hmi/screenshots/` for the README.
