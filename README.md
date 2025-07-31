# 🎮 2048 - Remake in C++ with SFML

A complete remake of the classic **2048** game, developed in **C++** using the **SFML** library.  
This project provides a modern experience with smooth animations, dark mode, score saving, and a compact UI designed for a **420x460 pixel** screen.

---

## 🔧 Features

- 🎨 **Graphical interface** (no terminal)
- 🌑 **Dark mode** with a custom design
- 🔢 **Current score** and **Best score** (saved to file)
- 🧠 **Animations** for tile movement and merging
- 🎉 **Victory screen** (when reaching tile 2048)
- 💀 **Game Over screen** + **Try Again** button
- 📏 **Optimized** for small resolutions: `420x460 px`

---

## 🎮 Controls

| Key | Action               |
|-----|----------------------|
| W   | Move up              |
| A   | Move left            |
| S   | Move down            |
| D   | Move right           |
| Q   | Quit the game        |

---

## ▶️ How to run the project

1. Make sure you have **SFML 2.5.1** installed.
2. Compile with:
   ```bash
   g++ -std=c++17 src/main.cpp src/game.cpp -o main.exe -lsfml-graphics -lsfml-window -lsfml-system
3. Run with:
   ```bash
   .\main.exe 
