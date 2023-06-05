#pragma once

#include<iostream>
#include<vector>
#include<ctime>
#include<sstream>

#include <SFML/Graphics.hpp>
#include <SFML/System.hpp>
#include <SFML/Window.hpp>
#include <SFML/Audio.hpp>
#include <SFML/Network.hpp>


/*
    Class that acts as the game engine.
*/

enum class GameState {
    MENU,
    PLAYING,
    END
};

class Game
{
private:
    sf::Font font;
    std::vector<sf::Text> menu;
    sf::RectangleShape play;//zzz

    //Variables
    //Window
    sf::RenderWindow* window;
    sf::VideoMode videoMode;
    sf::Event ev;
    //music
    sf::Music backgroundMusic;
    sf::Music alternateMusic;
    sf::Music alternateMusic2;
    //background
    sf::Texture backgroundTexture;
    sf::Sprite backgroundSprite;
    sf::Sprite alternateBackgroundSprite;
    sf::Texture alternateBackgroundTexture;
    sf::Sprite alternateBackgroundSprite2;
    sf::Texture alternateBackgroundTexture2;
    //Mouse positions
    sf::Vector2i mousePosWindow;
    sf::Vector2f mousePosView;

    //Text
    sf::Text uiText;

    //Game logic
    bool endGame;
    unsigned points;
    int health;
    float enemySpawnTimer;
    float enemySpawnTimerMax;
    int maxEnemies;
    bool mouseHeld;
    int lastScore;
    int startingHealth = 20;  // Default starting health

    //Game objects
    std::vector<sf::RectangleShape> enemies;
    sf::RectangleShape enemy;
    sf::CircleShape healthCircle;//healing enemy
    std::vector<sf::CircleShape> circles;//circle


    //Private functions
    void initVariables();
    void initWindow();
    void initFonts();
    void initText();
    void initEnemies();

public:
    //gamestate variable
    GameState gameState;

    //Constructors / Destructors
    Game();
    virtual ~Game();

    //Accessors
    const bool running() const;
    const bool getEndGame() const;

    //Functions
    void spawnEnemy();
    void mainLoop();//x
    void playGame();//d
    void resetGame();//x
    void pollEvents();
    void updateMousePositions();
    void updateText();
    void updateEnemies();
    void update();
    void changeStartingHealth();

    void renderText(sf::RenderTarget& target);
    void renderEnemies(sf::RenderTarget& target);
    void render();

    // New functions
    void showMenu();
};
