A simple 2D Space Shooter game developed using C++ and SFML, where the player controls a spaceship to fight off incoming enemies. 
The goal is to survive as long as possible, shoot enemies, and achieve a high score while avoiding collisions.
Features:
Player-controlled spaceship movement
Shooting mechanics
Enemy spawning and movement
Collision detection
Score tracking
Game-over condition
How to Run
Requirements:
C++ Compiler (g++)
SFML 2.5 or later
Steps:
Install SFML on your system.
Clone the repository:
git clone <repository-link>
Compile the project:
g++ main.cpp -o space_shooter -lsfml-graphics -lsfml-window -lsfml-system -lsfml-audio
Run the game:
./space_shooter
Note: SFML .dll files are required at runtime on Windows but are not included in this repository. Place them in the executable directory after building.
