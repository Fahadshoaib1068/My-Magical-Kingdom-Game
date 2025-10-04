# 🏰 My Magic Kingdom Game (C++ SFML Adventure)

An immersive **2D fantasy battle and adventure game** built entirely in **C++ using SFML**.  
Players explore a magical kingdom, train mythical pets, battle opponents, and rise through the ranks of the arena in this feature-packed console-graphic hybrid game.

This project showcases advanced use of **Object-Oriented Programming, SFML graphics, combat mechanics, AI logic, and dynamic memory management** — all designed from scratch.

---

## 🧙‍♂️ Overview

**My Magic Kingdom** brings an imaginative fantasy world to life through C++.  
You can choose and train your pets, shop for power-ups, engage in turn-based battles, participate in training games, and even challenge AI opponents or join guild wars.

The game is divided into interactive systems, including:
- 🐉 Pet Battle Arena  
- 🛍️ Item Shop  
- 🧩 Training Camp  
- 🧠 AI-controlled Opponents  
- ⚔️ Multi-Round Combat System  
- 🎮 Player Save/Load System  

---

## ✨ Key Features

### 🐾 Pet System
- Choose from multiple **unique pet classes**, each with distinct attributes and special abilities:
  - **Dragon** – high damage fire attacks  
  - **Griffin** – balanced attack and speed  
  - **Phoenix** – healing and rebirth ability  
  - **Cerberus** – tank-type defense and multiple strikes  
- Each pet has **health, mana, attack, defense, and stamina** stats.  
- Pets evolve through battles and training.

### ⚔️ Battle Mechanics
- Real-time keyboard-based controls:
  - `A` → Attack  
  - `H` → Heal  
  - `1`, `2`, etc. → Trigger special abilities  
- Player vs AI battles with **turn-based logic and effects**.  
- Supports 1v1, 2v2, and **Guild Team Battles**.  
- Includes **visual ASCII and SFML effects** like fireballs, speed lines, and hit animations.  
- Dynamic combat feedback (damage numbers, HP bars, cooldown timers).

### 🧩 Training Camp
- Integrated **mini-game** where players dodge obstacles or hit targets.  
- Performance translates into **stat boosts** (HP, Mana, Attack, Defense).  
- Reinforces skill and reaction timing — not just chance.

### 🛍️ Shop & Inventory System
- Buy and manage potions, mana restorers, and equipment.  
- Earn coins through battles and training.  
- Inventory stored dynamically using player save files.

### 🧠 AI System
- AI opponents pick strategies dynamically (attack, heal, or defend).  
- Varying difficulty levels.  
- Enemy pets are randomly selected for unpredictability.

### 💾 Save & Load System
- Automatically saves player data:
  - Player name  
  - Pet selection  
  - Wins, losses, and stats  
- Load saved progress anytime.

### 🎨 Graphics & UI
- Uses **SFML** for rendering menus, pets, backgrounds, and animations.  
- Includes ASCII effects for console users.  
- Clean, interactive menu design.

---

## 🛠️ Technical Implementation

| Feature | Description |
|----------|-------------|
| **Language** | C++ |
| **Graphics Library** | SFML (Simple and Fast Multimedia Library) |
| **Paradigm** | Object-Oriented Programming (OOP) |
| **Memory Management** | Manual DMA (Dynamic Memory Allocation) |
| **Input System** | Real-time keyboard input with `getch()` and SFML event handling |
| **Data Persistence** | File I/O (player profiles, saved games) |
| **Game Design Pattern** | Multi-class inheritance hierarchy for extensibility |

---

## 🧱 Major Classes

| Class | Purpose |
|--------|----------|
| `Pet` | Base class for all creature types (Dragon, Griffin, Phoenix, etc.) |
| `Arena` | Handles battles, input, and combat results |
| `Shop` | Allows item purchase and stat management |
| `Player` | Manages user data, pet ownership, and progression |
| `Power` | Applies buffs or boosts from training |
| `ObstaclesAndTraining` | Implements mini-game obstacle course |
| `Guild` | Coordinates 2v2 or multi-pet battles |
| `SaveSystem` | Loads and saves player files |
| `AI` | Controls enemy actions during fights |

---

## 🧩 Game Flow

1. **Main Menu**
   - Play Game  
   - Training Camp  
   - Visit Shop  
   - Guild Battle  
   - Load Game  
   - Exit  

2. **Choose Your Pet**
   - Dragon 🐉  
   - Griffin 🦅  
   - Phoenix 🔥  
   - Cerberus 🐺  

3. **Enter Battle Arena**
   - Fight against random or saved AI pets  
   - Use abilities strategically to win  

4. **Earn Rewards**
   - Gain XP and coins  
   - Buy upgrades or heal your pet  

5. **Optional Training**
   - Play obstacle mini-game for stat boosts  

6. **Save Progress**
   - Continue your journey next time!  

---

## ⚙️ Installation & Run

### Requirements
- **Windows OS**
- **C++ Compiler** supporting C++98 or later (MinGW, MSVC, or Code::Blocks)
- **SFML 2.5 or later** (for graphics and sound)

### Steps
   ```bash
   git clone https://github.com/your-username/MyMagicKingdom.git
   cd MyMagicKingdom
   g++ "my magic kingdom game.cpp" -o kingdom -lsfml-graphics -lsfml-window -lsfml-system
   kingdom.exe
   ```

## 🏆 Highlights

1. Built entirely from scratch — no STL vectors or engines
2. Real-time SFML interface with color, sprites, and sound
3. Deep OOP architecture with inheritance and polymorphism
4. Combines combat, economy, AI, and simulation systems
5. Designed and coded by a single developer 💪

## 👤 Author
Made by @Fahadshoaib1068
