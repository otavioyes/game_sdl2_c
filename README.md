# 2D Space Shooter — C + SDL2

> A modular 2D arcade shooter developed in pure C using SDL2.

Focused on:
- gameplay programming
- rendering architecture
- memory management
- entity systems
- modular game architecture

---

# Gameplay

![Gameplay GIF](docs/gameplay.gif)

---

# Features

- Modular gameplay architecture
- Entity system
- Rotating player ship
- Enemy AI
- Projectile system
- Collision detection
- Particle explosions
- Debris system
- HUD and highscores
- Audio system with SDL_mixer
- Texture cache system
- State-based game flow
- Bitmap font rendering
- Starfield background system
- Arcade-style gameplay loop

---

# Architecture

The project is separated into independent gameplay systems:

```text
src/
├── background.c
├── bullet.c
├── draw.c
├── effects.c
├── enemy.c
├── highscore.c
├── hud.c
├── init.c
├── input.c
├── main.c
├── player.c
├── points.c
├── sound.c
├── stage.c
├── text.c
├── title.c
└── util.c