#include <iostream>
#include <fstream>
using namespace std;
#include "sfml/Graphics.hpp"
#include "SFML/Audio.hpp"
struct Player { //struct is basically user defined type
    sf::Sprite sprite; // makes a sprite which will be used to load images
    sf::Texture texture; //makes a texture which will load textures for the game
    float x = 0.f;
    float y = 0.f; //handles the x and y coordinates 
};
struct Bullet {//another struct that deals with the sprite and texture and the speed of the bullets
    sf::Sprite sprite;
    sf::Texture texture;
    bool active = false;
    float speed = -10.f;
};
struct Enemy {
    sf::Sprite sprite;
    sf::Texture texture;
    bool active = false; // this tracks if the sprite is currently on screen or not
    float x = 0.0f, y = 0.0f;

};
void initialiseLevelDisplay(sf::Text& levelText, sf::Font& font) {
    if (!font.loadFromFile("arial.ttf")) {
        cout << "Error: Unable to load font for level display!" << endl;
        exit(EXIT_FAILURE);
    }
    levelText.setFont(font);
    levelText.setCharacterSize(30);
    levelText.setFillColor(sf::Color::White);
    levelText.setPosition(700.f, 20.f); // Centered at the top
}
bool initialisePlayer(Player& player, const string textureFile, float startX, float startY) {
    //takes the struct player by reference and initialises the variables
    if (!player.texture.loadFromFile(textureFile)) {
        cout << "Error... Unable to load player texture file!" << endl;
        return false;
    }
    player.sprite.setTexture(player.texture);
    player.x = startX;
    player.y = startY;
    player.sprite.setPosition(player.x, player.y);
    return true;
}
bool initialiseBullet(Bullet& bullet, const string texturefile) {
    //this just loads the texture files for the Bullet
    if (!bullet.texture.loadFromFile(texturefile)) {
        cout << "Error... unable to load bullet texture file!" << endl;
        return false;
    }
    bullet.sprite.setTexture(bullet.texture);
    return true;
}
bool initialiseEnemy(Enemy& enemy, const std::string& textureFile) {
    if (!enemy.texture.loadFromFile(textureFile)) {
        cout << "Error: Unable to load enemy texture!" << endl;
        return false;
    }
    enemy.sprite.setTexture(enemy.texture);
    return true;
}
void initialiseScoreDisplay(sf::Text& scoreText, sf::Font& font) { // current score being displayed
    if (!font.loadFromFile("arial.ttf")) { // takes the ttf file format and if not available, exits
        cout << "Error... Unable to load font!" << endl;
        exit(EXIT_FAILURE);
    }
    scoreText.setFont(font);
    scoreText.setCharacterSize(30);
    scoreText.setFillColor(sf::Color::White);
    scoreText.setPosition(1000.f, 20.f); // Top-right corner
}
void initialiseLivesDisplay(sf::Text& livesText, sf::Font& font) {// reads the font and displays the initial lives
    if (!font.loadFromFile("arial.ttf")) {
        cout << "Error: Unable to load font for lives display!" << endl;
        exit(EXIT_FAILURE);
    }
    livesText.setFont(font);
    livesText.setCharacterSize(30);
    livesText.setFillColor(sf::Color::White);
    livesText.setPosition(20.f, 20.f); // Top-left corner
}

void updateLivesDisplay(sf::Text& livesText, int lives) { //takes the initial number of lives and then converts it to string and outputs it
    livesText.setString("Lives: " + to_string(lives));
}
void displayLevelUpScreen(sf::RenderWindow& window, int level, sf::Font& font) { //dispalys the level up screen with a pause
    //render window is the main window with by reference font taken
    sf::Text levelUpText;
    levelUpText.setFont(font);
    levelUpText.setCharacterSize(50);
    levelUpText.setFillColor(sf::Color::Yellow);
    levelUpText.setString("Level " + std::to_string(level));
    levelUpText.setPosition(window.getSize().x / 2.f - levelUpText.getGlobalBounds().width / 2.f, window.getSize().y / 2.f - levelUpText.getGlobalBounds().height / 2.f);
    // by this, we can set the position exactly in the center of the screen
    window.clear();
    window.draw(levelUpText);
    window.display();

    // Pause for 1 second
    sf::sleep(sf::seconds(1));
}
void checkLifeLoss(Player& player, Enemy enemies[], int maxEnemies, int& lives, int screenHeight) {
    //checks if the players life has to be reduced, also checks to see if the sprite is out of bounds
    for (int i = 0; i < maxEnemies; ++i) {
        if (enemies[i].active) {
            // Check if the enemy collides with the player
            if (player.sprite.getGlobalBounds().intersects(enemies[i].sprite.getGlobalBounds())) {//built in functions
                enemies[i].active = false; // Deactivate the enemy
                lives--; // Decrease lives
                cout << "Collision detected! Lives left: " << lives << endl;
                break; // Only lose one life per frame
            }

            // Check if the enemy reaches the bottom of the screen
            if (enemies[i].sprite.getPosition().y > screenHeight) {// checking by > as as we move down the y val increases
                enemies[i].active = false; // Deactivate the enemy
                lives--; // Decrease lives
                cout << "Enemy reached the bottom! Lives left: " << lives << endl;
                break; // Only lose one life per frame
            }
        }
    }
}
void updateScoreDisplay(sf::Text& scoreText, int score) { //takes the score text value by reference and updates it
    scoreText.setString("Score: " + to_string(score));//converts the integer to a string and concatenates
}

void spawnEnemy(Enemy enemies[], int maxEnemies, const std::string& textureFile, int screenWidth) {
    for (int i = 0; i < maxEnemies; ++i) {
        if (!enemies[i].active) { // Find an inactive enemy
            enemies[i].active = true;
            enemies[i].x = rand() % (screenWidth - 50); // Random X position
            enemies[i].y = -50; // Start slightly above the screen
            enemies[i].sprite.setPosition(enemies[i].x, enemies[i].y);
            break; // Spawn one enemy at a time
        }
    }
}
void moveEnemies(Enemy enemies[], int maxEnemies, float enemySpeed, int screenHeight) {
    for (int i = 0; i < maxEnemies; ++i) {
        if (enemies[i].active) {
            enemies[i].y += enemySpeed; // Move the enemy down
            enemies[i].sprite.setPosition(enemies[i].x, enemies[i].y);//each sprite is directed to each entry in the array

            // Deactivate the enemy if it moves off-screen
            if (enemies[i].y > screenHeight) {
                enemies[i].active = false;
            }
        }
    }
}

void drawEnemies(sf::RenderWindow& window, const Enemy enemies[], int maxEnemies) {
    for (int i = 0; i < maxEnemies; ++i) {
        if (enemies[i].active) {
            window.draw(enemies[i].sprite);//checks if the status is on to draw the sprite
        }
    }
}

void drawPlayer(sf::RenderWindow& window, Player& player) {
    player.sprite.setPosition(player.x, player.y);
    window.draw(player.sprite);//draws the sprite of the player
}
void movePlayer(Player& player, float dx, float dy, const sf::RenderWindow& window) {
    float newX = player.x + dx;
    float newY = player.y + dy;
    float windowWidth = window.getSize().x;
    float windowHeight = window.getSize().y;
    float spriteHeight = player.sprite.getGlobalBounds().height;
    float spriteWidth = player.sprite.getGlobalBounds().width;
    if (newX < 0) newX = 0; // Left boundary
    if (newY < 0) newY = 0; // Top boundary
    if (newX + spriteWidth > windowWidth) newX = windowWidth - spriteWidth; // Right boundary
    if (newY + spriteHeight > windowHeight) newY = windowHeight - spriteHeight; // Bottom boundary
    player.x = newX;
    player.y = newY;
}
void shootBullet(Bullet bullets[], int maxBullets, const Player& player, const Bullet& bulletPrototype) {
    for (int i = 0; i < maxBullets; ++i) {
        // using the active part of teh struct to see whether the bullet is availabke for use or not
        if (!bullets[i].active) {
            bullets[i] = bulletPrototype;// sets the free bullet to the prototype bullet with all the loaded textures
            bullets[i].active = true;// shows that this bullet is now in use
            bullets[i].sprite.setPosition(player.x + player.sprite.getGlobalBounds().width / 2.f - bullets[i].sprite.getGlobalBounds().width / 2.f,
                player.y);
            // the divide by 2 center aligns the bullets to make sure that they are always coming from within the center of the sprite
            break;
        }
    }
}
void updateBullets(Bullet bullets[], int maxBullets, const sf::RenderWindow& window) {
    for (int i = 0; i < maxBullets; ++i) {
        if (bullets[i].active) {
            bullets[i].sprite.move(0, bullets[i].speed);//this is a function that updates only the y position of the bullet
            if (bullets[i].sprite.getPosition().y < 0) { // out of bounds
                bullets[i].active = false; //available for use again
            }
        }
    }
}
void drawBullets(sf::RenderWindow& window, const Bullet bullets[], int maxBullets) {
    for (int i = 0; i < maxBullets; ++i) {
        if (bullets[i].active) {
            window.draw(bullets[i].sprite); //attaches the image to the bullet
        }
    }
}
void checkCollisions(Bullet bullets[], int bulletCount, Enemy enemies[], int enemyCount, int& score) {
    //checks if the bullet has hit the enemy, if yes then the score is added to by 1
    sf::SoundBuffer enemyBuffer;
    if (!enemyBuffer.loadFromFile("destroy.wav")) {
        cout << "Error: Unable to load shoot sound!" << endl;

    }
    sf::Sound destroySound;
    destroySound.setBuffer(enemyBuffer); //loads the audio for life loss
    for (int i = 0; i < bulletCount; i++) {
        for (int j = 0; j < enemyCount; j++) {
            if (bullets[i].active && enemies[j].active) {
                if (bullets[i].sprite.getGlobalBounds().intersects(enemies[j].sprite.getGlobalBounds())) {
                    // Deactivate bullet and enemy
                    bullets[i].active = false;
                    enemies[j].active = false;
                    destroySound.play();
                    score += 1; // Increase score
                    break; // Check next bullet
                }
            }
        }
    }
}
void resetEnemies(Enemy enemies[], int maxEnemies) {// this is to reset the sprites outside of the screen whenever we level up
    for (int i = 0; i < maxEnemies; ++i) {
        enemies[i].active = false; // Deactivate all enemies
        enemies[i].x = 0.0f;
        enemies[i].y = -50.0f; // Move them off-screen
        enemies[i].sprite.setPosition(enemies[i].x, enemies[i].y);
    }
}
void checkLevelProgression(int& score, int& level, int& scoreToNextLevel, float& enemySpeed, sf::RenderWindow& window, sf::Font& font, Enemy enemies[], int maxEnemies) {
    //takes byref all the values so that they can be updated accodingly
    //also at every level up, a screen is displayed along with enemies being reset
    if (score >= scoreToNextLevel) {
        level++;
        scoreToNextLevel = level * 10;
        enemySpeed += 100.f; // Increase enemy speed with each level
        cout << "Level Up! Current Level: " << level << endl;

        // Show the level-up screen
        displayLevelUpScreen(window, level, font);
        resetEnemies(enemies, maxEnemies);
    }
}
void resetGame(Player& player, Bullet bullets[], int maxBullets, Enemy enemies[], int maxEnemies, int& score, int& level, int& lives, int& scoreToNextLevel, float& enemySpeed) {
    //resets the entire game when escape is pressed  
     // Reset player position
    player.x = 500.f;
    player.y = 500.f;
    player.sprite.setPosition(player.x, player.y);

    // Reset bullets
    for (int i = 0; i < maxBullets; ++i) {
        bullets[i].active = false;
    }

    // Reset enemies
    resetEnemies(enemies, maxEnemies);

    // Reset score, level, lives, and speed
    score = 0;
    level = 1;
    lives = 3;
    enemySpeed = 100.0f;
    scoreToNextLevel = level * 10;

    cout << "Game has been reset!" << endl;
}

void updateLevelDisplay(sf::Text& levelText, int level) {
    levelText.setString("Level: " + std::to_string(level)); //with each passing level, it is updated by adding one
}

void showInstructions(sf::RenderWindow& window, sf::Font& font) { // a new window is created that displays the instructions
    sf::Text instructions;
    instructions.setFont(font);
    instructions.setCharacterSize(30);
    instructions.setFillColor(sf::Color::White);
    instructions.setString("Instructions:\n"
        "1. Use Arrow Keys to move the player.\n"
        "2. Press Space to shoot bullets.\n"
        "3. Avoid enemy collisions and enemies reaching the bottom.\n"
        "4. Press ESC to reset the game.\n"
        "5. Survive and score points to progress through levels.\n\n"
        "Press BACKSPACE to return to the main menu.");
    instructions.setPosition(100.f, 100.f);

    while (window.isOpen()) {
        sf::Event event;
        while (window.pollEvent(event)) {
            if (event.type == sf::Event::Closed) {
                window.close();
            }
            if (sf::Keyboard::isKeyPressed(sf::Keyboard::Backspace)) {
                return; // Return to the main menu
            }
        }

        window.clear();
        window.draw(instructions);
        window.display();
    }
}

enum GameState { MAIN_MENU, INSTRUCTIONS, GAME }; //groups the common constants that have to be used 

void loadHighScore(int& highScore) { // a text file that puts the highscore integer(initialised to 0) 
    std::ifstream file("highscore.txt"); //open the text file
    if (file.is_open()) {
        file >> highScore; //takes the highscore from the file
        file.close();
    }
    else {
        // If file doesn't exist, set high score to 0
        highScore = 0;
    }
}
void saveHighScore(int highScore) {
    std::ofstream file("highscore.txt");
    if (file.is_open()) {
        file << highScore; //highscire is stored
        file.close();
    }
}
void updateScoreDisplay(sf::Text& scoreText, int score, int highScore) {
    scoreText.setString("Score: " + to_string(score) + "  High Score: " + to_string(highScore)); // updates the highscore
}
void showGameOver(sf::RenderWindow& window, sf::Font& font, GameState& gameState, int score, int highscore) {
    //updates and checks the highscore
    sf::Text gameOverText;
    gameOverText.setFont(font);
    gameOverText.setCharacterSize(50);
    gameOverText.setFillColor(sf::Color::Red);
    gameOverText.setString("GAME OVER");
    gameOverText.setPosition(window.getSize().x / 2.f - gameOverText.getGlobalBounds().width / 2.f, 200.f);//aligns in center   

    sf::Text scoreText;
    scoreText.setFont(font);
    scoreText.setCharacterSize(30);
    scoreText.setFillColor(sf::Color::White);
    scoreText.setString("Score: " + std::to_string(score));
    scoreText.setPosition(window.getSize().x / 2.f - scoreText.getGlobalBounds().width / 2.f, 300.f);

    sf::Text highScoreText;
    highScoreText.setFont(font);
    highScoreText.setCharacterSize(30);
    highScoreText.setFillColor(sf::Color::White);
    highScoreText.setString("High Score: " + std::to_string(highscore));
    highScoreText.setPosition(window.getSize().x / 2.f - highScoreText.getGlobalBounds().width / 2.f, 350.f);

    sf::Text restartText;
    restartText.setFont(font);
    restartText.setCharacterSize(30);
    restartText.setFillColor(sf::Color::White);
    restartText.setString("Press ENTER to return to the Main Menu");
    restartText.setPosition(window.getSize().x / 2.f - restartText.getGlobalBounds().width / 2.f, 400.f);

    sf::Text exitText;
    exitText.setFont(font);
    exitText.setCharacterSize(30);
    exitText.setFillColor(sf::Color::White);
    exitText.setString("Press ESC to exit");
    exitText.setPosition(window.getSize().x / 2.f - exitText.getGlobalBounds().width / 2.f, 450.f);

    while (window.isOpen()) {
        sf::Event event;
        while (window.pollEvent(event)) {
            if (event.type == sf::Event::Closed) {
                window.close();
                return;
            }

            if (event.type == sf::Event::KeyPressed) {
                if (event.key.code == sf::Keyboard::Enter) {
                    gameState = MAIN_MENU;
                    return; //return back to the main menu
                }
                if (event.key.code == sf::Keyboard::Escape) {
                    window.close();
                    return; //pressing escape to end the game
                }
            }
        }

        window.clear();
        window.draw(gameOverText);
        window.draw(scoreText);
        window.draw(highScoreText);
        window.draw(restartText);
        window.draw(exitText);
        window.display();
    }
}
int main() {
    int highScore = 0;  // To store the highest score
    loadHighScore(highScore);

    // Create game window
    sf::RenderWindow window(sf::VideoMode(1410, 750), "Space Shooter");
    sf::Font font;
    if (!font.loadFromFile("arial.ttf")) {
        cout << "Error: Unable to load font!" << endl;
        return -1;
    }
    sf::Music introMusic; //loads the intro music
    if (!introMusic.openFromFile("cinematic-intro-6097.wav")) {
        cout << "Error: Unable to load intro music!" << endl;
        return -1;
    }
    introMusic.setLoop(true);
    introMusic.play();
    sf::SoundBuffer shootBuffer;
    if (!shootBuffer.loadFromFile("shoot.wav")) {
        cout << "Error: Unable to load shoot sound!" << endl;
        return -1;
    }
    sf::Sound shootSound;
    shootSound.setBuffer(shootBuffer); //loads the audio for laser shooting



    GameState gameState = MAIN_MENU;
    while (window.isOpen()) {
        sf::Event event;
        while (window.pollEvent(event)) {
            if (event.type == sf::Event::Closed) {
                window.close();
            }
        }

        if (gameState == MAIN_MENU) {
            // Display the intro screen
            sf::Text title, option1, option2;

            // Title
            title.setFont(font);
            title.setCharacterSize(50);
            title.setFillColor(sf::Color::Yellow);
            title.setString("SPACE SHOOTER");
            title.setPosition(window.getSize().x / 2.f - title.getGlobalBounds().width / 2.f, 100.f);

            // Option 1: Start Game
            option1.setFont(font);
            option1.setCharacterSize(30);
            option1.setFillColor(sf::Color::White);
            option1.setString("1. Start Game");
            option1.setPosition(window.getSize().x / 2.f - option1.getGlobalBounds().width / 2.f, 250.f);

            // Option 2: Instructions
            option2.setFont(font);
            option2.setCharacterSize(30);
            option2.setFillColor(sf::Color::White);
            option2.setString("2. Instructions");
            option2.setPosition(window.getSize().x / 2.f - option2.getGlobalBounds().width / 2.f, 320.f);

            window.clear();
            window.draw(title);
            window.draw(option1);
            window.draw(option2);
            window.display();

            // Handle keyboard input
            if (sf::Keyboard::isKeyPressed(sf::Keyboard::Num1)) {
                introMusic.stop();
                gameState = GAME;
            }
            else if (sf::Keyboard::isKeyPressed(sf::Keyboard::Num2)) {
                introMusic.stop();
                gameState = INSTRUCTIONS;
            }

        }
        else if (gameState == INSTRUCTIONS) {
            showInstructions(window, font);
            introMusic.play(); // Resume music when returning to the intro screen
            gameState = MAIN_MENU;
        }
        else if (gameState == GAME) {


            // Initialize the player
            Player player;
            if (!initialisePlayer(player, "spaceship.png", 500.f, 500.f)) {
                return -1;
            }
            int lives = 3;

            // Initialize bullet prototype
            Bullet bulletPrototype;
            if (!initialiseBullet(bulletPrototype, "laser.png")) {
                return -1;
            }



            // Initialize enemies
            const int maxEnemies = 10;
            Enemy enemies[maxEnemies] = {};
            for (int i = 0; i < maxEnemies; ++i) {
                if (!initialiseEnemy(enemies[i], "enemy.png")) {
                    return -1;
                }
            }

            // Initialize bullet array and the live and level display
            const int maxBullets = 5;
            Bullet bullets[maxBullets] = {};
            int score = 0;
            int level = 1;
            float enemySpeed = 100.f;
            int scoreToNextLevel = level * 10;

            sf::Text scoreText;
            sf::Text livesText;
            sf::Text levelText;
            initialiseLevelDisplay(levelText, font);
            initialiseLivesDisplay(livesText, font);
            initialiseScoreDisplay(scoreText, font);




            // Timing variables
            sf::Clock gameClock;          // For deltaTime
            sf::Clock bulletClock;        // For bullet cooldown
            sf::Clock spawnClock;         // For enemy spawning
            sf::Time fireCooldown = sf::milliseconds(500); // Bullet cooldown
            sf::Time spawnDelay = sf::seconds(1.5f);       // Enemy spawn delay
            sf::Time lastFireTime;

            while (window.isOpen() && gameState == GAME) { //to see if the game is loaded rn
                sf::Event event;
                while (window.pollEvent(event)) {
                    if (event.type == sf::Event::Closed) {
                        window.close();
                    }
                }
                if (sf::Keyboard::isKeyPressed(sf::Keyboard::Escape)) {
                    resetGame(player, bullets, maxBullets, enemies, maxEnemies, score, level, lives, scoreToNextLevel, enemySpeed);
                    //mid game if the user presses escape, the game is reset to level 1
                }
                // Calculate deltaTime
                float deltaTime = gameClock.restart().asSeconds();

                // Player movement
                if (sf::Keyboard::isKeyPressed(sf::Keyboard::Left)) {
                    movePlayer(player, -400.f * deltaTime, 0.f, window);
                }
                if (sf::Keyboard::isKeyPressed(sf::Keyboard::Right)) {
                    movePlayer(player, 400.f * deltaTime, 0.f, window);
                }
                if (sf::Keyboard::isKeyPressed(sf::Keyboard::Up)) {
                    movePlayer(player, 0.f, -400.f * deltaTime, window);
                }
                if (sf::Keyboard::isKeyPressed(sf::Keyboard::Down)) {
                    movePlayer(player, 0.f, 400.f * deltaTime, window);
                }

                // Bullet firing
                if (sf::Keyboard::isKeyPressed(sf::Keyboard::Space)) {
                    sf::Time currentTime = bulletClock.getElapsedTime();
                    if (currentTime - lastFireTime >= fireCooldown) {
                        shootBullet(bullets, maxBullets, player, bulletPrototype);
                        shootSound.play();
                        lastFireTime = currentTime;
                    }
                }


                // Enemy spawning
                if (spawnClock.getElapsedTime() >= spawnDelay) {
                    spawnEnemy(enemies, maxEnemies, "enemy.png", 1200);
                    spawnClock.restart();
                }

                // Update enemies and bullets
                moveEnemies(enemies, maxEnemies, enemySpeed * deltaTime, 900); //this ensures that a periodic time is followed
                updateBullets(bullets, maxBullets, window);
                checkCollisions(bullets, maxBullets, enemies, maxEnemies, score);
                checkLifeLoss(player, enemies, maxEnemies, lives, window.getSize().y);
                checkLevelProgression(score, level, scoreToNextLevel, enemySpeed, window, font, enemies, maxEnemies);
                updateScoreDisplay(scoreText, score);
                updateLivesDisplay(livesText, lives);
                if (lives <= 0) {
                    std::cout << "Game Over!" << std::endl;
                    showGameOver(window, font, gameState, score, highScore);

                    // Restart the intro music when returning to main menu
                    introMusic.play();
                    if (score > highScore) {
                        highScore = score;  // Update the high score
                        saveHighScore(highScore);  // Save to file
                    }

                    // Continue the main loop to handle the game state change
                    break;
                }

                updateLevelDisplay(levelText, level);
                updateScoreDisplay(scoreText, score, highScore);

                // Rendering
                window.clear();
                drawEnemies(window, enemies, maxEnemies);
                drawPlayer(window, player);
                drawBullets(window, bullets, maxBullets);
                window.draw(scoreText);
                window.draw(livesText);
                window.draw(levelText);
                window.display();
            }
            continue;

        }
    }
    return 0;
}