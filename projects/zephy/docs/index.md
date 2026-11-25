<div class="hero" markdown>
<p class="eyebrow">Personal project · AI assistant · 9th upgrade</p>

# Zephy

<p class="pitch">My personal voice assistant. It started as a local assistant on an Ollama / Qwen backend and is now a Jarvis-style agent on my own machine that can place calls, schedule meetings and work on projects with me and others.</p>

<p class="tag-row"><span>Local LLM</span><span>Ollama</span><span>Qwen</span><span>Voice assistant</span><span>Telegram</span><span>Tailscale</span></p>

[Architecture](#architecture){ .md-button .md-button--primary } [Evolution](#evolution){ .md-button }
</div>

## What Zephy does

- Places calls and schedules meetings
- Works on projects with me and with other people
- Runs locally on hardware I own, with no paid API keys

## Architecture

```mermaid
flowchart LR
  ME[Me: text or voice] -->|Telegram| AG[Zephy agent]
  AG --> LLM[Local LLM<br/>Ollama · Qwen]
  AG --> TOOLS[Tools: calls, calendar, browser, files]
  subgraph Home server
    AG
    LLM
    TOOLS
  end
  ME -. remote access .-> TS[Tailscale] -.-> AG
```

| Piece | Choice |
| --- | --- |
| Host | ASUS Zephyrus G14 (32 GB RAM, RTX 4070) converted into a 24/7 home AI server |
| Model backend | Local models via Ollama (Qwen family) |
| Interface | Messaging through Telegram, voice on the machine |
| Remote access | Tailscale |
| Wake behaviour | Sleeps until messaged; a light heartbeat checks email and only pings me when something matters |

## Evolution

Zephy began as a local assistant answering questions on an Ollama / Qwen backend. Nine upgrades later it is an agent that takes actions: calls, scheduling and project work, from anywhere.

## Next

- Move the home server from Windows to Ubuntu
- Shared memory and tool layer so every assistant I use sees the same knowledge
- Edge hardware (Jetson) for always-on, low-power operation
