# Troubleshooting

| Symptom | Cause | Fix |
| --- | --- | --- |
| Collector: `BadNodeIdUnknown` against the real PLC | DB not published, or names differ from the contract | CPU > OPC UA > Server active; DB properties > *Accessible from HMI/OPC UA*; names exactly as `docs/05_opcua_interface.md` |
| Collector cannot connect to PLCSIM | Plain PLCSIM has no network interface | Use **PLCSIM Advanced** with a virtual Ethernet adapter (PLCSIM Virtual Ethernet Adapter / TCP/IP communication mode) and use the instance's IP |
| Connection refused by the PLC's security policy | S7-1500 default server settings refuse anonymous / unsigned clients | For the lab, allow *No security* and guest access in the CPU's OPC UA server settings; for anything else, set up certificates |
| Writes from `cellsim.cmd` rejected by the real PLC | `DB_HMI` not writable over OPC UA | Tick *Writable from HMI/OPC UA* on `DB_HMI` |
| Dashboard says "No data yet" | Collector not running, or a different `--db` path | Start the collector; pass the same `--db` to both |
| Dashboard time axis moves faster than real time | Mock running with `--speed` > 1 | Expected: chart axes are wall clock, durations are PLC time |
| Port 4840 already in use | Another OPC UA server (or an old mock) running | Stop it, or use `--endpoint opc.tcp://0.0.0.0:4841` and the same port in `--url` |
| `pytest` end-to-end test fails on port 48410 | Port busy | Free the port or change `PORT` in `tests/test_end_to_end.py` |
| Factory I/O actuators do not move | Driver not running, or the PLC in STOP | Factory I/O Drivers window shows green; PLCSIM Advanced instance in RUN |
| TIA trial ran out mid-build | 21-day limit | Restore the last daily `.zap` on a licensed or lab PC (`docs/06_tia_build_plan.md`) |
