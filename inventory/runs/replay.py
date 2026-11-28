"""Replay an expert demonstration from a Minari dataset and save a video.
Usage: python replay.py D4RL/door/expert-v2      (also D4RL/hammer/expert-v2, D4RL/kitchen/complete-v2)
Install: pip install "minari[hdf5]" gymnasium-robotics "imageio[ffmpeg]"
"""
import sys, imageio, minari

name = sys.argv[1] if len(sys.argv) > 1 else "D4RL/door/expert-v2"
ds = minari.load_dataset(name, download=True)
env = ds.recover_environment(render_mode="rgb_array")
for ep in ds.iterate_episodes(range(min(5, ds.total_episodes))):
    env.reset()
    frames, success = [], False
    for action in ep.actions:
        _, _, terminated, truncated, info = env.step(action)
        frames.append(env.render())
        success |= bool(info.get("success") or len(info.get("episode_task_completions", [])) >= 4)
        if terminated or truncated:
            break
    print(f"episode {ep.id}: success={success}")
    if success:
        out = name.split("/")[1] + ".mp4"
        imageio.mimwrite(out, frames, fps=30)
        print("Saved", out)
        break
