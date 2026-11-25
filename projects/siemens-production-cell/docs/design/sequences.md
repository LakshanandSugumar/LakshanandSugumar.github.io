# S7-GRAPH sequences

Generated from `python/cellsim/sequences.py` by `python/tools/gen_docs.py`. Edit the Python file, then regenerate, so the simulator and this spec never drift.

Conventions, matching how the sequences are built in S7-GRAPH:

- **Action** = what the step does (N = non-stored action unless stated).
- **Transition** = condition to leave the step.
- **Interlock (C)** = must stay TRUE for the step's actions to run. If it drops, the actions stop and an interlock error is shown; the step does not advance.
- **Supervision (V)** = the step faults if its transition is not met in time: `V: S_NO_TIME > T#<limit>`. The fault code becomes the alarm number.
- Wait steps have no supervision: a starved or blocked station is not a fault.
- Ideal cycle time used for OEE performance: 6.0 s per part.

## Station 1: Infeed and inspection (`GR_Station1`)

| Step | Name | Action | Transition | Interlock (C) | Nominal (s) | Supervision (V) | Fault |
| --- | --- | --- | --- | --- | --- | --- | --- |
| S1 | Wait for part | Infeed conveyor ON | S1_PartAtEntry | Mode=Auto AND NOT Cell_Fault | wait | - | - |
| S2 | Convey to vision | Infeed conveyor ON | S1_PartAtVision | Mode=Auto AND NOT Cell_Fault | 1.5 | 4 s | 101 Part did not reach the vision sensor |
| S3 | Read part type | Infeed conveyor OFF, latch S1_VisionType | S1_VisionType <> 0 | Mode=Auto AND NOT Cell_Fault | 0.3 | 1 s | 102 Vision sensor read timeout |
| S4 | Extend reject pusher | S1_RejectPusher ON | S1_PusherFront | Infeed conveyor OFF AND S1_PusherBack was TRUE | 0.6 | 2 s | 103 Reject pusher did not reach front position |
| S5 | Retract reject pusher | S1_RejectPusher OFF | S1_PusherBack | Mode=Auto AND NOT Cell_Fault | 0.6 | 2 s | 104 Reject pusher did not return to back position |
| S6 | Wait for station 2 ready | - | S2_ReadyForPart | Mode=Auto AND NOT Cell_Fault | wait | - | - |
| S7 | Transfer to station 2 | Infeed and transfer conveyors ON | S2_BaseAtStop | S2_StopBlade UP AND S1_PusherBack | 1.2 | 4 s | 105 Part transfer to station 2 timed out |

Branch after S3: wrong part type goes S4 -> S5 -> S1 (reject, RejectCount + 1); right part goes S6 -> S7 -> S1.

## Station 2: Pick and place assembly (`GR_Station2`)

| Step | Name | Action | Transition | Interlock (C) | Nominal (s) | Supervision (V) | Fault |
| --- | --- | --- | --- | --- | --- | --- | --- |
| S1 | Wait for base | - | S2_BaseAtStop | Mode=Auto AND NOT Cell_Fault | wait | - | - |
| S2 | Clamp base at stop | S2_StopBlade UP, S2_Clamp ON | S2_Clamped | S2_BaseAtStop | 0.8 | 3 s | 201 Base not clamped at assembly stop |
| S3 | Move X to lid pick | S2_PP_X to PICK | S2_PP_XAtPick | S2_PP_ZAtTop | 0.8 | 2 s | 202 Pick and place X did not reach pick position |
| S4 | Lower Z to lid | S2_PP_Z DOWN | S2_PP_ZAtBottom | S2_PP_XAtPick AND NOT S2_PP_Detected | 0.5 | 1.5 s | 203 Pick and place Z did not reach bottom at pick |
| S5 | Grip lid | S2_PP_Grab ON | S2_PP_Detected | S2_PP_ZAtBottom | 0.3 | 1 s | 204 Lid not detected in gripper |
| S6 | Raise Z with lid | S2_PP_Z UP | S2_PP_ZAtTop | S2_PP_Detected | 0.5 | 1.5 s | 205 Pick and place Z did not return to top |
| S7 | Move X to place | S2_PP_X to PLACE | S2_PP_XAtPlace | S2_PP_ZAtTop AND S2_Clamped | 0.8 | 2 s | 206 Pick and place X did not reach place position |
| S8 | Lower Z and release lid | S2_PP_Z DOWN, then S2_PP_Grab OFF | S2_PP_ZAtBottom AND NOT S2_PP_Detected | S2_PP_XAtPlace AND S2_Clamped | 0.8 | 2 s | 207 Lid not released on base |
| S9 | Raise Z and home X | S2_PP_Z UP, S2_PP_X to PICK | S2_PP_ZAtTop AND S2_PP_XAtPick | - | 0.8 | 2 s | 208 Pick and place did not return home |
| S10 | Wait for station 3 free | - | S3_ReadyForPart | Mode=Auto AND NOT Cell_Fault | wait | - | - |
| S11 | Unclamp and release part | S2_Clamp OFF, S2_StopBlade DOWN, conveyor ON | S3_PartAtEntry | S2_PP_ZAtTop | 0.6 | 2 s | 209 Assembled part did not leave station 2 |

S11 -> S1 when the part has left. Station 2 is the bottleneck (6.7 s nominal).

## Station 3: Outfeed and count (`GR_Station3`)

| Step | Name | Action | Transition | Interlock (C) | Nominal (s) | Supervision (V) | Fault |
| --- | --- | --- | --- | --- | --- | --- | --- |
| S1 | Wait for part | - | S3_PartAtEntry | Mode=Auto AND NOT Cell_Fault | wait | - | - |
| S2 | Convey to counter | Outfeed conveyor ON | S3_PartCount | Mode=Auto AND NOT Cell_Fault | 1.5 | 4 s | 301 Part not seen at outfeed counter |
| S3 | Count and remove | Increment GoodCount, outfeed ON | S3_PartAtExit falling edge | Mode=Auto AND NOT Cell_Fault | 0.4 | 1 s | 302 Part did not clear the outfeed |

S3 -> S1 and GoodCount + 1 on the falling edge of S3_PartAtExit.

## Handshakes between sequences

| Signal | Set by | Reset by | Meaning |
| --- | --- | --- | --- |
| S2_ReadyForPart | GR_Station2 in S1 | GR_Station2 leaving S2 | Station 2 can accept a base |
| S3_ReadyForPart | GR_Station3 in S1 | GR_Station3 leaving S2 | Station 3 can accept a part |

Each sequence only reads the other's ready bit; no output is written by two sequences.
