Skizzagatchi

A tiny virtual-pet inspired project built using an ESP32-S3 SuperMini, a 0.49" OLED display (64×32), and a simple button switch.
Skizzagatchi is a pocket-sized frog creature that eats, sleeps, levels up, and comes to life on an incredibly small screen.

🐸 Project Overview

Skizzagatchi is a minimalist Tamagotchi-style creature designed to run on extremely small hardware.
Despite the tiny 64×32 OLED, it includes:

Animated idle frog (with blinking)

Sleep cycle with pillow, bubbles, and drifting “Z” graphics

Tiny twinkling stars while sleeping

XP system with leveling every 10 minutes of awake time

Level bar with outline

Level-up sparkle explosion animation

Automatic sleep/wake behavior

Fully optimized pixel art (16×16 frog sprites)

🧠 How It Works

The frog stays awake for several minutes at a time, blinking randomly and idling.
After enough awake time, it gets tired and enters sleep mode:

Frog rotates 90° sideways

Pillow appears under its head

A bubble rises from its mouth

Three Z’s drift upward

Small one-pixel stars twinkle at the top of the screen

While awake, the frog earns XP.
Every 10 minutes, it levels up and triggers a sparkle explosion effect.

The bottom of the screen shows a level progress bar, and the top-left displays the current level number.

🧩 Hardware Used

ESP32-S3 SuperMini

0.49" OLED Display (64×32, SSD1306)

1× Button Switch (for future menus/interactions)

📦 Features Ready for Expansion

Skizzagatchi is designed to be easily modded.
You can add:

Hunger & feeding

Mood system

Happiness and care meters

More animations (jumping, hopping, eating)

Button-driven menus

Custom pixel pets or skins

🛠 Code

All animations are drawn using Adafruit GFX and SSD1306 libraries.
The project is designed to be memory-light and ESP32-friendly, with efficient timers and sprite handling.

Wiring:
| Device     | Pin       | ESP32-S3 Pin |
| ---------- | --------- | ------------ |

| **OLED**   | VCC       | **3.3V**     |
|            | GND       | **GND**      |
|            | SCL       | **GPIO 9**   |
|            | SDA       | **GPIO 8**   |

| **Button** | One leg   | **GPIO 0**   |
|            | Other leg | **GND**      |

![image alt](https://github.com/scoutpoltorak-wq/skizzagatchi/blob/b95f2fa5e6a06efe2af84abdd25204e5a2e1890f/IMG_3879.jpg)


