# 60-90 s demo video shot list

1. (5 s) Hardware overview: motor, encoder, Nucleo + driver, CANable, e-stop. Title card.
2. (10 s) Screen split: sim step response vs hardware step response overlaid (`compare_sim`).
3. (10 s) Position steps and sine tracking, live plot next to the moving inertia disk.
4. (10 s) Feedforward on/off: tracking error drops (show the RMS number).
5. (15 s) Collision demo: joint swings into a foam block, stops on contact; plot shows the observer spike.
6. (15 s) Encoder fault demo: pull the encoder connector (or trigger the injected freeze) while spinning; joint stops, STATUS shows ENC_MODEL.
7. (5 s) Comms watchdog: kill the Python script, joint goes safe.
8. (5 s) End card: repo link, one line on what is next (fault-tolerant control).

Film at 60 fps with the plot on a second monitor, or capture the screen and the
bench separately and edit side by side.
