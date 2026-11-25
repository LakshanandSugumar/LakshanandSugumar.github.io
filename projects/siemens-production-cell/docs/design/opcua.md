# OPC UA interface

Generated from `python/cellsim/tags.py` by `python/tools/gen_docs.py`.

Namespace URI: `http://www.siemens.com/simatic-s7-opcua`. Node ids are `ns=<index of that URI>;s="DB"."Variable"`, which is how an S7-1500 publishes global DB variables. Resolve the index by URI at connect time; do not hard-code `ns=3`.

In TIA Portal: CPU properties > OPC UA > Server > Activate, then set the DBs below as *Accessible from HMI/OPC UA* (and *Writable* for DB_HMI). `DB_Sim` exists only in the mock server.

| Node (string id) | PLC type | Access | Description |
| --- | --- | --- | --- |
| `"DB_Production"."CellState"` | Int | read | 0 Stopped, 1 Running, 2 Fault |
| `"DB_Production"."CellMode"` | Int | read | 1 Auto, 2 Manual, 3 Setup (mirror of DB_HMI.Mode) |
| `"DB_Production"."GoodCount"` | DInt | read | Good parts at outfeed since counter reset |
| `"DB_Production"."RejectCount"` | DInt | read | Parts rejected at inspection |
| `"DB_Production"."TotalCount"` | DInt | read | GoodCount + RejectCount |
| `"DB_Production"."LastCycleTime_ms"` | DInt | read | Time between the last two good parts |
| `"DB_Production"."IdealCycleTime_ms"` | DInt | read | Design cycle time per part |
| `"DB_Production"."PlannedTime_s"` | Real | read | Time in Auto with the cell started |
| `"DB_Production"."RunTime_s"` | Real | read | Planned time minus fault downtime |
| `"DB_Production"."ActiveFaultCode"` | Int | read | 0 when no fault is active |
| `"DB_Production"."ActiveFaultStation"` | Int | read | Station of the active fault |
| `"DB_Production"."ActiveFaultStep"` | Int | read | GRAPH step of the active fault |
| `"DB_Production"."FaultSeq"` | DInt | read | Increments when a fault starts |
| `"DB_Production"."FaultClearedSeq"` | DInt | read | Increments when a fault is reset |
| `"DB_Production"."LastFaultCode"` | Int | read | Code of the last cleared fault |
| `"DB_Production"."LastFaultStation"` | Int | read | Station of the last cleared fault |
| `"DB_Production"."LastFaultDuration_s"` | Real | read | Downtime of the last cleared fault |
| `"DB_Production"."S1_ActiveStep"` | Int | read | Active GRAPH step, station 1 |
| `"DB_Production"."S2_ActiveStep"` | Int | read | Active GRAPH step, station 2 |
| `"DB_Production"."S3_ActiveStep"` | Int | read | Active GRAPH step, station 3 |
| `"DB_HMI"."Mode"` | Int | read/write | Requested mode, 1 Auto, 2 Manual, 3 Setup |
| `"DB_HMI"."Cmd_Start"` | Bool | read/write | Start pulse |
| `"DB_HMI"."Cmd_Stop"` | Bool | read/write | Stop pulse |
| `"DB_HMI"."Cmd_Reset"` | Bool | read/write | Fault reset / acknowledge pulse |
| `"DB_Sim"."InjectFaultStation"` | Int | read/write | Write 1-3 to jam the current step of that station |

Enumerations: CellState 0 Stopped, 1 Running, 2 Fault. Mode 1 Auto, 2 Manual, 3 Setup.

Event pattern: the PLC increments `FaultClearedSeq` after it has written `LastFaultCode`, `LastFaultStation` and `LastFaultDuration_s`. The collector subscribes to the sequence counter and then reads the three detail tags, so a client never sees half-written fault data.
