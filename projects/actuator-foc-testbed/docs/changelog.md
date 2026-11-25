# Changelog

## 0.1.0 (2026-11-11)

- Simulation: surface-PMSM plant with sensor models, cascaded FOC controller, fault monitor, 8 experiments, demo video renderer, regression tests.
- Firmware core (C99): FOC math, cascade, back-EMF residual and momentum-observer fault detection, CAN protocol, AS5047P driver, encoder calibration mode. Host unit tests and software-in-the-loop.
- STM32G474 + IHM07M1 HAL glue and CubeMX settings.
- Host tools: python-can link with a 1 kHz logger, test profiles, encoder calibration, sim-vs-hardware comparison, and a virtual joint on the real protocol.
- Documentation: getting started, design and theory, CAN protocol, system identification, research ideas.
- CI: GitHub Actions runs the Python and firmware tests and regenerates the figures.
