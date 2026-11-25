# Cell design and I/O list

Three stations on one conveyor line with a reject lane, built as a custom Factory I/O scene.

```
 Emitter ─► [S1 Infeed + vision] ─► transfer ─► [S2 Pick & place assembly] ─► [S3 Outfeed + count] ─► Remover
                    │                                    ▲ lids from a second emitter
                    └─► reject pusher ─► reject lane
```

| Station | Job | Key parts in Factory I/O |
| --- | --- | --- |
| 1 Infeed and inspection | Detect part, read type with the vision sensor, push wrong parts to the reject lane | Emitter (bases, mixed with wrong parts), roller conveyor, diffuse sensors, vision sensor, pusher |
| 2 Pick and place assembly | Clamp the base at a stop, pick a lid, place it on the base, release | Stop blade, clamp, 2-axis pick and place, lid emitter on a feeder conveyor |
| 3 Outfeed and count | Count good parts, remove them | Roller conveyor, diffuse sensors, remover |

Station 2 is the bottleneck (about 6.7 s nominal per part). The ideal cycle used for OEE is 6.0 s.

## Building the scene

1. Start a new empty scene. Use the stock *Assembler* and *Sorting by Height* scenes only as references for how the pick and place and pusher are wired.
2. Place parts in the order of the table above. Name every sensor and actuator in Factory I/O exactly as the tag below; it makes the driver mapping one-to-one.
3. Set the base emitter to mix in about 5 % of a different part (e.g. blue vs green raw material) so the vision step has something to reject.
4. Drivers: choose **Siemens S7-PLCSIM Advanced**, select the instance, and map the tags to the addresses below.
5. Demo fault: in Factory I/O you can force a sensor (right-click > Force). Forcing `S1_PusherFront` FALSE jams the pusher and trips supervision 103 after 2 s.

Save the scene as `factoryio/production_cell.factoryio`.

## I/O list

Also in `io_list.csv` for import into the TIA tag table.

| Tag | Address | Type | Station | Factory I/O part | Description |
| --- | --- | --- | --- | --- | --- |
| Cell_Start | %I0.0 | Bool | Panel | Start button | Start pulse (NO) |
| Cell_Stop | %I0.1 | Bool | Panel | Stop button | Stop (NC) |
| Cell_Reset | %I0.2 | Bool | Panel | Reset button | Fault reset / acknowledge |
| Cell_EStop | %I0.3 | Bool | Panel | Emergency stop | E-stop chain (NC; FALSE = stopped) |
| Cell_AutoSel | %I0.4 | Bool | Panel | Selector switch | Auto selected (FALSE = Manual) |
| S1_PartAtEntry | %I1.0 | Bool | 1 | Diffuse sensor | Part present at infeed |
| S1_PartAtVision | %I1.1 | Bool | 1 | Diffuse sensor | Part in front of vision sensor |
| S1_PusherFront | %I1.2 | Bool | 1 | Pusher (front limit) | Reject pusher extended |
| S1_PusherBack | %I1.3 | Bool | 1 | Pusher (back limit) | Reject pusher retracted |
| S1_VisionType | %IW10 | Int | 1 | Vision sensor (numerical) | Part ID from vision sensor (0 = none) |
| S2_BaseAtStop | %I2.0 | Bool | 2 | Diffuse sensor | Base at assembly stop |
| S2_Clamped | %I2.1 | Bool | 2 | Clamp (limit) | Base clamped |
| S2_PP_XAtPick | %I2.2 | Bool | 2 | Pick and place X (limit) | X at lid pick position |
| S2_PP_XAtPlace | %I2.3 | Bool | 2 | Pick and place X (limit) | X at place position |
| S2_PP_ZAtTop | %I2.4 | Bool | 2 | Pick and place Z (limit) | Z at top |
| S2_PP_ZAtBottom | %I2.5 | Bool | 2 | Pick and place Z (limit) | Z at bottom |
| S2_PP_Detected | %I2.6 | Bool | 2 | Pick and place (item detected) | Lid held in gripper |
| S3_PartAtEntry | %I3.0 | Bool | 3 | Diffuse sensor | Part entered outfeed |
| S3_PartCount | %I3.1 | Bool | 3 | Diffuse sensor | Counting sensor |
| S3_PartAtExit | %I3.2 | Bool | 3 | Diffuse sensor | Part at remover |
| S1_ConvInfeed | %Q1.0 | Bool | 1 | Roller conveyor | Infeed conveyor run |
| S1_ConvTransfer | %Q1.1 | Bool | 1 | Roller conveyor | Transfer conveyor to station 2 |
| S1_RejectPusher | %Q1.2 | Bool | 1 | Pusher | Extend reject pusher |
| S2_StopBlade | %Q2.0 | Bool | 2 | Stop blade | Raise stop blade |
| S2_Clamp | %Q2.1 | Bool | 2 | Clamp | Clamp base |
| S2_PP_X | %Q2.2 | Bool | 2 | Pick and place X | TRUE = move to place / FALSE = to pick |
| S2_PP_Z | %Q2.3 | Bool | 2 | Pick and place Z | TRUE = down |
| S2_PP_Grab | %Q2.4 | Bool | 2 | Pick and place grab | Gripper on |
| S2_ConvAssembly | %Q2.5 | Bool | 2 | Roller conveyor | Assembly conveyor run |
| S3_ConvOutfeed | %Q3.0 | Bool | 3 | Roller conveyor | Outfeed conveyor run |
| Cell_Light_Green | %Q4.0 | Bool | Panel | Stack light green | Running |
| Cell_Light_Yellow | %Q4.1 | Bool | Panel | Stack light yellow | Stopped / manual / waiting reset |
| Cell_Light_Red | %Q4.2 | Bool | Panel | Stack light red | Fault |
