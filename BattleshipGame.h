// //
// // Created by Muhammad Benoun on 19/12/2024.
// //
// // GameEntity.h
#pragma once
#include <SFML/Graphics.hpp>

#ifndef GAME_ENTITY_H
#define GAME_ENTITY_H


#include <vector>





class BattleshipGame {
private:
    std::unique_ptr<int> shipLeft = std::make_unique<int>(8);
    std::unique_ptr<int> healthLeft = std::make_unique<int>(5);
    sf::RenderWindow window;
    sf::Texture shipTexture, boomTexture, redHartTexture, blackHartTexture, backgroundTexture;
    sf::Sprite backgroundSprite;
    sf::Font font;
    sf::Text welcomeText, restartButtonText;
    std::vector<sf::RectangleShape> rectangles;
    sf::RectangleShape restartButton; // Button shape


    int posIndex[5][5] =  { {1, 0, 1, 0, 2},
                            {2, 1, 0, 0, 0},
                            {0, 1, 0, 2, 0},
                            {0, 1, 0, 1, 0},
                            {2, 0, 1, 2, 1}};


    int posRows = 5, indexSize = 5;  // Rows and columns based on posIndex


public:
    BattleshipGame();
    bool initTextures(); // Initializes all textures
    bool initFont(); // Initializes the font
    void createRectangles(); // Creates the rectangles based on grid
    void handleEvents(); // Handles events such as mouse clicks
    void update(); // Updates the game state (health, position)
    void render(); // Draws everything to the window
    void runGame(); // Main game loop
    void resetGame();
};








#endif // GAME_ENTITY_H
