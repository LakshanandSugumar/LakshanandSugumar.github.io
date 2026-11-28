"""Run a pretrained expert policy in a Gymnasium MuJoCo task and save a video.
Usage: python walk.py Humanoid-v5        (also Ant-v5, HalfCheetah-v5, Hopper-v5, Pusher-v5, HumanoidStandup-v5)
Install: pip install "gymnasium[mujoco]" stable-baselines3 huggingface_hub "imageio[ffmpeg]"
"""
import sys, imageio, mujoco, gymnasium as gym
from huggingface_hub import hf_hub_download
from stable_baselines3 import SAC, PPO

env_id = sys.argv[1] if len(sys.argv) > 1 else "Humanoid-v5"
algo = "PPO" if env_id.startswith("HumanoidStandup") else "SAC"
path = hf_hub_download(f"farama-minari/{env_id}-{algo}-expert", f"{env_id.lower()}-{algo.lower()}-expert.zip")
model = {"SAC": SAC, "PPO": PPO}[algo].load(path, device="cpu", custom_objects={"learning_rate": 0.0, "lr_schedule": lambda _: 0.0})

env = gym.make(env_id, max_episode_steps=1000)
m = env.unwrapped.model
m.vis.global_.offwidth, m.vis.global_.offheight = 960, 540
renderer, cam = mujoco.Renderer(m, 540, 960), mujoco.MjvCamera()
if env_id.startswith("Pusher"):
    cam.lookat[:] = [0, -0.3, 0]; cam.distance, cam.elevation, cam.azimuth = 2.2, -40, 90
else:
    cam.type, cam.trackbodyid = mujoco.mjtCamera.mjCAMERA_TRACKING, 1
    cam.distance, cam.elevation, cam.azimuth = 4.5, -14, 135

obs, _ = env.reset(seed=0)
stride = max(1, round((1 / env.unwrapped.dt) / 30))
frames, total = [], 0.0
for t in range(1000):
    action, _ = model.predict(obs, deterministic=True)
    obs, reward, terminated, truncated, _ = env.step(action)
    total += reward
    if t % stride == 0:
        renderer.update_scene(env.unwrapped.data, cam)
        frames.append(renderer.render())
    if terminated or truncated:
        break
out = env_id.split("-")[0].lower() + ".mp4"
imageio.mimwrite(out, frames, fps=30)
print(f"{env_id}: return {total:.0f} over {t + 1} steps, fell={terminated}. Saved {out}")
