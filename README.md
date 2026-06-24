# Space Shooter Game (C++ / SFML)

A beginner-friendly 2D arcade-style space shooter built with **C++** and **SFML**. Pilot a spaceship, blast incoming enemies, dodge collisions, and rack up points across increasingly difficult levels — with sound effects, music, a main menu, and a persistent high score.

## Features

- **Player-controlled spaceship** — smooth, boundary-clamped movement
- **Shooting mechanics** — fire lasers with a cooldown to prevent spamming
- **Enemy spawning & movement** — enemies spawn at random horizontal positions and move downward
- **Collision detection** — bullet-vs-enemy hits score points; enemy-vs-player or enemy-reaching-bottom costs a life
- **Leveling system** — speed and difficulty increase as your score crosses each level threshold, with a "Level Up" screen
- **Lives system** — game ends after 3 lives are lost
- **Persistent high score** — saved to and loaded from `highscore.txt`
- **Audio** — intro music, laser shoot sound, and enemy destruction sound
- **Menu system** — main menu, in-game instructions screen, and a game-over screen with restart/exit options

## Project Structure

```
.
├── space shooter game.cpp     # Main game source
├── arial.ttf                   # Font used for all on-screen text
├── spaceship.png                # Player sprite
├── enemy.png                    # Enemy sprite
├── laser.png                    # Bullet sprite
├── shoot.wav                    # Laser firing sound effect
├── destroy.wav                  # Enemy destruction sound effect
├── cinematic-intro-6097.wav     # Main menu background music
└── highscore.txt                # Stores the persisted high score
```

## Controls

| Key | Action |
|---|---|
| **Arrow Keys** | Move the spaceship |
| **Space** | Fire a bullet |
| **Esc** | Reset the current game (mid-run) |
| **1** | Start Game (from main menu) |
| **2** | Instructions (from main menu) |
| **Backspace** | Return to main menu (from instructions screen) |
| **Enter** | Return to main menu (from game-over screen) |
| **Esc** | Exit (from game-over screen) |

## How to Play

1. From the **Main Menu**, press `1` to start playing or `2` to view instructions.
2. Use the **arrow keys** to move your spaceship and **Space** to shoot.
3. Destroy enemies before they collide with you or reach the bottom of the screen — each costs a life.
4. Score points with every enemy you destroy. Crossing a score threshold (`level × 10`) triggers a **Level Up**, increasing enemy speed.
5. Survive as long as you can — the game ends when your lives reach 0, showing your final score against the all-time high score.

## Requirements

- A C++ compiler (`g++` recommended)
- **SFML 2.5 or later** (Graphics, Window, System, and Audio modules)

## How to Build & Run

1. **Install SFML** on your system (see the [SFML downloads page](https://www.sfml-dev.org/download.php) for your platform).
2. **Clone the repository:**
   ```bash
   git clone https://github.com/Fatima-26-23/Space-Shooter-Game.git
   cd Space-Shooter-Game
   ```
3. **Compile the project:**
   ```bash
   g++ "space shooter game.cpp" -o space_shooter -lsfml-graphics -lsfml-window -lsfml-system -lsfml-audio
   ```
4. **Run the game:**
   ```bash
   ./space_shooter
   ```

> **Windows note:** SFML's `.dll` files are required at runtime but are **not included** in this repository. After building, copy the relevant SFML `.dll` files into the same folder as the executable.

> **Important:** Run the executable from the project's root directory (or copy it there) so it can find `arial.ttf`, the `.png` sprites, and the `.wav`/`.txt` asset files via their relative paths.

## How It Works (Under the Hood)

- **`Player`, `Bullet`, and `Enemy`** are simple structs, each holding an `sf::Sprite`/`sf::Texture` pair plus position and state (`active`, `speed`, etc.).
- **Object pooling** is used for bullets and enemies — fixed-size arrays (`maxBullets = 5`, `maxEnemies = 10`) are reused by toggling an `active` flag instead of constantly allocating/destroying objects.
- **`sf::Clock`** instances track delta time for frame-independent movement, bullet fire-rate cooldown, and enemy spawn intervals.
- **Collision detection** uses SFML's built-in `getGlobalBounds().intersects()` to check bullet-enemy and player-enemy overlaps.
- **Game state** is managed through a simple `enum GameState { MAIN_MENU, INSTRUCTIONS, GAME }`, with the main loop branching on the current state.
- **High score persistence** is handled with simple file I/O (`<fstream>`) reading/writing a single integer to `highscore.txt`.

## Known Limitations

- Asset and font paths are relative, so the executable must be run from the project folder.
- Fixed-size arrays cap simultaneous bullets/enemies (5 and 10 respectively) rather than using dynamic containers.
- No window resizing support — fixed at 1410×750.
- This is a learning project focused on demonstrating core game-loop and SFML concepts, not a polished commercial release.

## Possible Extensions

- Add power-ups (extra lives, faster fire rate, shields)
- Add varied enemy types or boss fights
- Replace fixed-size arrays with `std::vector`
- Add a pause menu and volume controls
- Support window resizing / fullscreen toggle

## Author

**Fatima** — Computer Science student, FAST-NUCES, Lahore
