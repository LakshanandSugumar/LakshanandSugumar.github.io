# Operating modes, interlocks and alarms

This follows the structure SICAR-style cells use: one mode manager for the cell, one GRAPH sequence per station, and the same interlocks guarding both automatic and manual motion.

## Modes and cell states

| Mode | Selected by | What runs | Allowed transitions |
| --- | --- | --- | --- |
| Auto | HMI or panel selector, cell stopped | All GRAPH sequences in automatic, Start / Stop from panel or HMI | to Manual or Setup only when stopped |
| Manual | HMI, cell stopped | GRAPH sequences switched to manual (`SW_MAN`); actuators jog from the Manual screen, each guarded by its interlock | to Auto only after a homing check (all actuators at their home limit) |
| Setup | HMI, cell stopped | GRAPH in *tap* mode (`SW_TAP`): one step per Start press, for commissioning | to Auto after homing check |

Cell state (published as `DB_Production.CellState`): **Stopped** (0), **Running** (1), **Fault** (2). A station fault holds the whole cell: every sequence stops advancing until Reset. This is a deliberate simplification; a real line often lets other stations empty out.

E-stop removes the `Cell_EStop` signal: all outputs drop, all sequences are reset to their initial step (`INIT_SQ`) after the E-stop is released and Reset is pressed. In Factory I/O this is a functional model only; real safety would be on a fail-safe F-CPU.

## Stack light

| Light | Meaning |
| --- | --- |
| Green steady | Auto, running |
| Yellow steady | Stopped, or Manual / Setup |
| Yellow flashing | Fault cleared, waiting for Start |
| Red flashing | Active fault, not acknowledged |
| Red steady | Fault acknowledged, condition still present |

## Manual-mode interlocks

Each jog button on the HMI is enabled only when its interlock is TRUE, and the HMI shows the interlock state next to the button so the operator sees why a move is blocked.

| Actuator | Jog | Interlock |
| --- | --- | --- |
| S1_RejectPusher | Extend | Infeed conveyor stopped |
| S1_ConvInfeed | Run | Pusher at back |
| S2_PP_Z | Down | X at pick OR X at place |
| S2_PP_X | Move | Z at top |
| S2_PP_Grab | Release | Z at bottom (never drop a lid from the top) |
| S2_StopBlade | Lower | Clamp open AND Z at top |
| S2_Clamp | Open | Z at top |
| S3_ConvOutfeed | Run | none |

## Alarms

Every supervision fault in `02_sequences.md` is one alarm, numbered by its fault code (101 to 302). Two ways to raise it in TIA, use the first:

1. **GRAPH supervision + ProDiag / system diagnostics.** With the GRAPH FBs set to report supervision errors, TIA creates the alarm with the step text automatically and the WinCC alarm view shows it with no extra code.
2. **Program alarm** (`Program_Alarm`) in `FB_Production`, triggered from `ActiveFaultCode`, if the automatic alarms are not available in your license.

Either way `FB_Production` also writes the fault into `DB_Production` (code, station, step, start time, duration) for the OPC UA dashboard.

Additional cell-level alarms (not from GRAPH):

| Code | Text |
| --- | --- |
| 1 | Emergency stop pressed |
| 2 | Mode change refused, cell running |
| 3 | Homing check failed, cannot enter Auto |
