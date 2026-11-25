# Changelog

## 0.1.0 (2026-11)

Everything that can be built before TIA Portal is available.

- Design docs: cell layout, I/O list, S7-GRAPH step specs, modes and interlocks, HMI spec, OPC UA contract, 21-day TIA build plan, research ideas.
- Mock S7-1500 OPC UA server running a time-stepped model of the three-station cell, with fault injection and HMI-style commands.
- OPC UA collector to SQLite, OEE / Pareto / MTBF / MTTR maths, Streamlit dashboard.
- SCL drafts for `DB_Production`, `DB_HMI`, `FB_ModeManager`, `FB_Production` (not yet compiled in TIA).
- 10 tests including a full server-to-SQLite run over OPC UA; GitHub Actions workflow.

## Next

- TIA Portal project, GRAPH sequences, WinCC screens, Factory I/O scene (Dec 14 to Jan 3).
- Collector pointed at PLCSIM Advanced; demo video.
