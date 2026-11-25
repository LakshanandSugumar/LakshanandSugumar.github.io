# Collector, data model and dashboard

```
PLC or mock ──OPC UA──► collector.py ──► data/cell.db (SQLite) ──► app.py (Streamlit, read-only)
```

The dashboard never connects to the PLC. Only the collector does, and it only reads, so the dashboard cannot change anything on the cell.

## Collector (`python/dashboard/collector.py`)

```bash
python -m dashboard.collector --url opc.tcp://localhost:4840 --db data/cell.db --period 1.0
```

- Resolves the namespace index from the Siemens URI `http://www.siemens.com/simatic-s7-opcua` at connect time, so the same code works on the mock and on PLCSIM Advanced.
- **Subscription** (100 ms publishing interval, queue 50) on `FaultClearedSeq` and `TotalCount`:
  - `FaultClearedSeq` changed → read `LastFaultCode`, `LastFaultStation`, `LastFaultDuration_s` → one row in `faults`. A jump of more than 1 is logged as missed records.
  - `TotalCount` rose → read `LastCycleTime_ms` → one row in `cycles`.
- **Polling** every `--period` seconds → one row in `snapshots` with all counters and states.

## SQLite schema

| Table | Columns | One row per |
| --- | --- | --- |
| `snapshots` | ts, planned_s, run_s, total, good, reject, ideal_ms, cell_state, mode, active_fault_code, active_fault_station, active_fault_step, s1_step, s2_step, s3_step | poll period |
| `cycles` | ts, total, cycle_ms | part counted |
| `faults` | ts, seq (unique), code, station, text, duration_s | fault cleared |

`ts` is wall-clock Unix time on the collector PC. Durations (`planned_s`, `run_s`, `cycle_ms`, `duration_s`) are PLC time, so they stay correct when the mock runs faster than real time.

## KPIs (`python/dashboard/oee.py`)

```
OEE = A × P × Q
A = RunTime / PlannedTime
P = IdealCycleTime × TotalCount / RunTime
Q = GoodCount / TotalCount
```

- **Rolling OEE** over N planned minutes uses counter deltas between the newest snapshot and the last snapshot at least N minutes of planned time earlier. Idle time outside Auto does not dilute the window.
- **OEE since reset** uses the absolute counters.
- **Pareto**: downtime summed per fault code, largest first, with cumulative share.
- **MTBF** per station = cell run time / faults at that station. **MTTR** = mean fault duration.
- Ideal cycle time is 6.0 s per part, set in `sequences.py` and published as `IdealCycleTime_ms`.

## Dashboard (`python/dashboard/app.py`)

```bash
streamlit run dashboard/app.py -- --db ../data/cell.db
```

Refreshes every 2 s. Contents, top to bottom:

1. Status banner: running, stopped, or the active fault with code, station, step and text.
2. KPI tiles: rolling OEE and its three factors, OEE since reset, good / reject counts.
3. Rolling OEE trend with an 85 % benchmark line, and cycle time per good part against the ideal.
4. Downtime Pareto by fault, MTBF / MTTR per station, and the active GRAPH step of each station.
5. Fault history table, newest first.

The OEE window (5 to 120 planned minutes) is set in the sidebar.

## Sample run

Mock at 30× speed, fault probability doubled to 0.004 so a short run shows enough faults, two injected faults, about 45 minutes of PLC time:

| Metric | Value |
| --- | --- |
| OEE (last 30 min) | 66.0 % |
| Availability / Performance / Quality | 72.5 % / 98.0 % / 93.0 % |
| Good / reject | 257 / 19 |
| Mean cycle time | about 6.5 s (ideal 6.0 s) |
| Top downtime fault | 301 Part not seen at outfeed counter |
| Most faults | Station 2 (9 faults, MTBF 3.2 min, MTTR 38 s) |

With the default fault probability, OEE settles above 80 %.
