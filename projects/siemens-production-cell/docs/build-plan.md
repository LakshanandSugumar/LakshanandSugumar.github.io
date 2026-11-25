# 21-day TIA Portal build plan

Use this if the build runs on the TIA Portal trial. Activate TIA Portal (and PLCSIM Advanced) and Factory I/O on the same morning; the TIA clock is the one that matters. Everything Python is already done and tested, so all 21 days are Siemens work.

**Every evening:** Project > Archive to `plc/ProductionCell_dayNN.zap`, commit, push. If the trial ends early, nothing is lost.

## Week 1: hardware, scene, I/O, manual mode

| Day | Goal | Done when |
| --- | --- | --- |
| 1 | New TIA project, CPU 1516-3 PN/DP, PLCSIM Advanced instance, tag table from `docs/io_list.csv`; import and compile the SCL drafts | Project compiles and downloads to PLCSIM Advanced |
| 2 | Build the Factory I/O scene (`docs/01_cell_design_and_io.md`), connect the PLCSIM Advanced driver, map all tags | Every sensor toggles its input in a watch table, every output moves its actuator |
| 3 | `FB_ModeManager` in OB1, stack light, E-stop chain | Mode change refused while running; E-stop drops all outputs |
| 4 | Manual jog logic with interlocks for all actuators (`docs/03_modes_and_interlocks.md`) | Every jog blocked when its interlock is FALSE |
| 5 | Buffer day / catch up; first video clip of manual mode | |

## Week 2: GRAPH sequences

| Day | Goal | Done when |
| --- | --- | --- |
| 6 | `GR_Station3` (simplest, 3 steps) to learn the editor, supervision, `ACK_EF` | Parts counted; forcing `S3_PartCount` FALSE raises 301 |
| 7 | `GR_Station1` with the reject branch | Wrong parts go to the reject lane; 101 to 105 trip when forced |
| 8-9 | `GR_Station2` (11 steps) and the handshakes | Cell runs parts end to end in Auto for 30 min |
| 10 | `FB_Production` in OB30; check counters, planned/run time against a stopwatch | Numbers in a watch table match `docs/05_opcua_interface.md` |
| 11 | Alarms: GRAPH supervision to ProDiag/system diagnostics, cell alarms 1-3 | Every fault code shows as an alarm with the right text |
| 12 | Buffer day | |

## Week 3: HMI, OPC UA, video

| Day | Goal | Done when |
| --- | --- | --- |
| 13-15 | WinCC screens 1-5 (`docs/04_hmi_spec.md`) | Whole demo can be run from the HMI only |
| 16 | OPC UA server on, DBs accessible; point the collector at PLCSIM Advanced (`--url opc.tcp://<instance IP>:4840`) | Dashboard shows live OEE from the real PLC program |
| 17 | Run the cell 2+ hours with faults forced now and then, to fill the dashboard | Pareto and MTTR have real data |
| 18 | Record the video (demo script in the main README) | Raw footage saved |
| 19 | Screenshots of every block, every HMI screen, GRAPH printouts to PDF | Files in `plc/graph_printouts/` and `hmi/screenshots/` |
| 20-21 | Buffer, final archive, export Factory I/O scene | Final `.zap` committed |
