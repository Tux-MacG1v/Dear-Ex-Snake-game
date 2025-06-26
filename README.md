# 🎮 Dear Ex Snake

**Dear Ex Snake** is a retro-style, console-based snake game written in C++ for Windows. With heartbreak humor and colorful animations, it’s a personalized twist on the classic arcade snake, made as part of a school undergraduate project in **Data Structures and Algorithms**.

---

## 💡 Description

This game offers a fun and emotional take on the traditional snake game:
- The player (a.k.a. **Victim**) controls a red snake representing their ex.
- Food (green) must be collected while avoiding walls and self-collision.
- The game starts with a themed countdown and ends with a colorful pixel-based "Game Over" box, showing score and stamina (time survived).

This project was developed as part of a **Data Structure and Algorithm course requirement** to demonstrate understanding of core data structures such as arrays, deques, and control loops within a real-time, interactive system.

---

## 🧠 Features

- 🟨 Yellow border, 🟥 Red snake, 🟩 Green food
- 🎬 Pixel-style intro screen with countdown (Get ready 3, 2, 1)
- ⌛ Live stamina tracker (how long you survived)
- 🎯 Score tracking with increasing speed
- 🎮 Controls: WASD or Arrow Keys
- 📦 Game Over animation with detailed recap

---

## 🖥️ Requirements

- Windows OS
- A C++ compiler that supports WinAPI (`windows.h`), e.g., Code::Blocks, Dev-C++, Visual Studio

---

## 📦 How to Run

1. Clone or download this repository.
2. Open `snake.cpp` in your C++ IDE (e.g., Code::Blocks).
3. Compile and Run.
4. Use keyboard to play:
   - `W` / `↑` – Up
   - `A` / `←` – Left
   - `S` / `↓` – Down
   - `D` / `→` – Right
   - `R` – Restart
   - `X` – Exit

---

## 🎥Gameplay Video

If you want to see the gameplay in action before trying it out, watch the video demo:
👉 [Watch it](https://github.com/Tux-MacG1v/Dear-Ex-Snake-game/blob/main/snake_gihub.mp4)

---

## 🧑‍💻 Project Info

- 👤 **Creator:** Tux-MacG1v
- 🎓 **Player:** Victim
- 🆔 **ID:** 456
- 🎮 **Title:** DEAR EX SNAKE
- 📘 **Course:** Data Structures and Algorithms

This project is part of an undergraduate course curriculum, showcasing real-time control, terminal UI, and structured programming using fundamental data structures.

---

---

## 🔎 Analysis

### Data Structures

- **`deque<Point>`**: Represents the snake body for efficient insertion/removal at both ends (O(1) per operation).
- **`Point` struct**: Stores X/Y coordinates for snake segments, food, and collision detection.
- **`mt19937` + `uniform_int_distribution`**: Used for pseudo-random food placement, seeded by a high-resolution clock.

### Algorithm Design

1. **Start Screen** (`showStartScreen`)  
   - Clears the console and draws a colored box with title, player info, and countdown.  
   - Uses console cursor positioning to overwrite countdown number in place.

2. **Game Loop** (`run`)  
   - Records `gameStart` time.  
   - While `alive`:  
     - Capture input (WASD/arrow keys) with `processInput` (non-blocking).  
     - Update snake position and state in `update`.  
     - Render the board and status in `render`.

3. **Input Handling** (`processInput`)  
   - Differentiates between arrow key codes and character keys, ensuring no 180° turns.

4. **Update Logic** (`update`)  
   - Moves head in current direction.  
   - Checks for collisions with walls or self (linear search).  
   - Grows snake when eating food, adjusts `delay` speed, and places new food.

5. **Rendering** (`render`)  
   - Uses WinAPI to position the cursor at (0,0) each frame for flicker-free updates.  
   - Draws borders, snake, food, and dynamic status text (score/stamina).

6. **Game Over** (`showGameOver`)  
   - Plays a dot-matrix animation.  
   - Displays a final summary box with score, stamina, and messages.

### Time Complexity

Let *n* be the snake length, *W×H* the board size:

- **Input & Movement**: O(1) per frame.
- **Collision Check**: O(n).
- **Food Placement**: O(n) worst-case.
- **Rendering**: O(W·H).

Overall per-frame: **O(W·H + n)**.

### Space Complexity

- **O(n + W·H)**: Snake segments plus console buffer.
- Additional O(1) for timer, score, and small helpers.

### Complexity Graph

Here's a more modern, text-based representation of per-frame time costs:

```
       Time per Frame
           ^
           |       ██████████████████████████  Rendering (O(W·H))
           |   ████                          
           |   ████ Collision & Placement (O(n))
           |   ██                             
           |__██______________________________> Snake Length (n)
             Low   Med    High
```
- **Rendering (O(W·H))**: Fixed plateau independent of snake length.
- **Collision & Placement (O(n))**: Grows linearly with snake length.

> Rendering dominates overall performance, especially on larger boards.

---

## 🛠️ Implementation Details

- **Language & API**: C++17 with Windows Console API (`<windows.h>`, `<conio.h>`).
- **Randomness**: Uses Mersenne Twister seeded by `chrono::steady_clock`.
- **Console Control**: Cursor hiding, ANSI codepage set to UTF-8 for Unicode blocks.
- **Timing**: Frame pacing via `std::this_thread::sleep_for`, aiming at ~10 FPS (`INITIAL_DELAY` ms).

---

## 📸 Screenshot

```
########################################
#             DEAR EX SNAKE           #
#   NAME: Victim       ID: 2324       #
# Use WASD or Arrow Keys to control   #
#        Get ready! 3, 2, 1...        #
########################################
```

---

## 📄 License

This project is released under the MIT License. See `LICENSE` file for more info.

---

> "The snake is red like my rage, green like my jealousy, and boxed in like my past."

Enjoy the chaos. ❤️🐍
