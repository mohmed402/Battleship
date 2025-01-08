// //
// // Created by Muhammad Benoun on 19/12/2024.
// //
//
// //GameEntity.cpp
//
#include "BattleshipGame.h"
#include <SFML/Graphics.hpp>
#include <vector>
#include <memory>
#include <iostream>
#include <random>

BattleshipGame::BattleshipGame() : window(sf::VideoMode(800, 600), "Battleship Game") {
    restartButton.setSize(sf::Vector2f(150, 30));
    restartButton.setFillColor(sf::Color::Cyan);
    restartButton.setPosition(320, 520);

    // Set up the button text
    restartButtonText.setFont(font);
    restartButtonText.setString("Restart");
    restartButtonText.setCharacterSize(20);
    restartButtonText.setFillColor(sf::Color::Black);
    restartButtonText.setPosition(363, 523);
}

bool BattleshipGame::initTextures() {
    // Load textures for ship, boom, and hearts
    if (!shipTexture.loadFromFile("/Users/muhammadbenoun/CLionProjects/battleShips/resources/ship.png") ||
        !boomTexture.loadFromFile("/Users/muhammadbenoun/CLionProjects/battleShips/resources/redBoom.jpeg") ||
        !redHartTexture.loadFromFile("/Users/muhammadbenoun/CLionProjects/battleShips/resources/hart.png") ||
        !blackHartTexture.loadFromFile("/Users/muhammadbenoun/CLionProjects/battleShips/resources/blackHart.png") ||
        !backgroundTexture.loadFromFile("/Users/muhammadbenoun/CLionProjects/battleShips/resources/background.png")) {
        std::cerr << "Error loading textures!" << std::endl;
        return false;
    }
    return true;
}



bool BattleshipGame::initFont() {
    if (!font.loadFromFile("/Users/muhammadbenoun/CLionProjects/battleShips/resources/ARIAL.TTF")) {
        std::cerr << "Error loading font!" << std::endl;
        return false;
    }
    return true;
}

void BattleshipGame::createRectangles() {
    int cellSize = 65; // Size of each rectangle
    int gridWidth = cellSize * indexSize;
    int gridHeight = cellSize * posRows;

    int offsetX = std::max(static_cast<int>((window.getSize().x - gridWidth) / 2), 0);
    int offsetY = std::max(static_cast<int>((window.getSize().y - gridHeight) / 2 + 50), 0);



    for (int j = 0; j < posRows; j++) {
        for (int i = 0; i < indexSize; i++) {
            sf::RectangleShape rectangle(sf::Vector2f(cellSize, cellSize));
            rectangle.setPosition(offsetX + cellSize * i, offsetY + cellSize * j);
            rectangle.setFillColor(sf::Color::White);
            rectangle.setOutlineThickness(3);
            rectangle.setOutlineColor(sf::Color::Black);
            rectangles.push_back(rectangle);
        }
    }
}
void BattleshipGame::handleEvents() {
    sf::Event event;
    while (window.pollEvent(event)) {
        if (event.type == sf::Event::Closed) {
            window.close();
        }

        if (event.type == sf::Event::MouseButtonPressed) {
            if (event.mouseButton.button == sf::Mouse::Left) {
                sf::Vector2i mousePos = sf::Mouse::getPosition(window);

                // Check if the restart button is clicked
                if (restartButton.getGlobalBounds().contains(sf::Vector2f(mousePos))) {
                    resetGame();
                    return;
                }

                // Check each rectangle
                for (size_t i = 0; i < rectangles.size(); i++) {
                    if (rectangles[i].getGlobalBounds().contains(sf::Vector2f(mousePos)) && *shipLeft > 0 && *healthLeft > 0) {
                        int& indexI = posIndex[i / indexSize][i % indexSize]; // Use a reference to modify the array
                        std::cout << "clicked: " << indexI << std::endl;
                        if (indexI == 1) { // Hit a ship
                            rectangles[i].setTexture(&shipTexture);
                            rectangles[i].setTextureRect(sf::IntRect(20, 20, shipTexture.getSize().x, shipTexture.getSize().y));
                            rectangles[i].setScale(
                                rectangles[i].getSize().x / shipTexture.getSize().x,
                                rectangles[i].getSize().y / shipTexture.getSize().y
                            );
                            rectangles[i].setScale(1.0f, 1.0f);
                            (*shipLeft)--;
                            indexI = 3; // Mark as already clicked
                        } else if (indexI == 2) { // Hit a bomb
                            rectangles[i].setTexture(&boomTexture);
                            rectangles[i].setTextureRect(sf::IntRect(0, 0, boomTexture.getSize().x, boomTexture.getSize().y));
                            rectangles[i].setScale(
                                rectangles[i].getSize().x / boomTexture.getSize().x,
                                rectangles[i].getSize().y / boomTexture.getSize().y
                            );
                            rectangles[i].setScale(1.0f, 1.0f);
                            (*healthLeft)--;
                            indexI = 3; // Mark as already clicked
                        }else if (indexI == 0) { // Empty water
                            rectangles[i].setFillColor(sf::Color::Cyan);
                            indexI = 3; // Mark as already clicked
                        }
                    }
                }
            }
        }
    }
}


void BattleshipGame::update() {
    if (*shipLeft <= 0) {
        welcomeText.setString("You Won!");
    } else if (*healthLeft <= 0) {
        welcomeText.setString("Game Over!");
    } else {
        welcomeText.setString("Welcome to Battleship!");
    }
}

void BattleshipGame::render() {
    backgroundSprite.setTexture(backgroundTexture);
    window.draw(backgroundSprite);
    window.draw(restartButton);
    window.draw(restartButtonText);
    // Draw all rectangles (if any)
    for (size_t i = 0; i < rectangles.size(); i++) {
        window.draw(rectangles[i]);
    }


    welcomeText.setFont(font);
    welcomeText.setCharacterSize(40);
    welcomeText.setFillColor(sf::Color::White);
    welcomeText.setStyle(sf::Text::Bold);

    // Calculate the position for the welcome text to center it
    sf::FloatRect welcomeTextBounds = welcomeText.getLocalBounds();
    welcomeText.setPosition(
        (window.getSize().x - welcomeTextBounds.width) / 2, 10);

    window.draw(welcomeText);

    // Draw health text
    sf::Text healthText;
    healthText.setFont(font);
    healthText.setString("Health: ");
    healthText.setCharacterSize(40);
    healthText.setFillColor(sf::Color::White);
    healthText.setStyle(sf::Text::Bold);

    healthText.setPosition(190, 80);
    window.draw(healthText);


    for (int h = 0; h < 5; h++) {
        sf::Sprite heartSprite;
        if (h < *healthLeft) {
            heartSprite.setTexture(redHartTexture);
            heartSprite.setScale(0.15f, 0.15f);
            heartSprite.setPosition(325 + h * 50, 82);
        } else {
            heartSprite.setTexture(blackHartTexture);
            heartSprite.setScale(0.10f, 0.10f);
            heartSprite.setPosition(325 + h * 50, 85);
        }
        window.draw(heartSprite);
    }


    window.display();
}

int getRandomNumber() {
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_int_distribution<> distrib(0, 4);
    return distrib(gen);
}


void BattleshipGame::resetGame() {

    *shipLeft = 8;
    *healthLeft = 5;

    // Clear the grid (0 = Empty)
    for (size_t row = 0; row < posRows; row++) {
        for (size_t col = 0; col < indexSize; col++) {
            posIndex[row][col] = 0;
        }
    }

    // Randomly place 8 ships
    int shipsPlaced = 0;
    while (shipsPlaced < 8) {
        int row = getRandomNumber() % posRows;
        int col = getRandomNumber() % indexSize;

        if (posIndex[row][col] == 0) { // Check if the cell is empty
            posIndex[row][col] = 1; // 1 = Ship
            shipsPlaced++;
        }
    }

    // Randomly place 5 booms
    int boomsPlaced = 0;
    while (boomsPlaced < 5) {
        int row = getRandomNumber() % posRows;
        int col = getRandomNumber() % indexSize;

        if (posIndex[row][col] == 0) { // Check if the cell is empty
            posIndex[row][col] = 2; // 2 = Boom
            boomsPlaced++;
        }
    }

    // Reset rectangles (clear any textures or customizations)
    for (auto& rect : rectangles) {
        rect.setTexture(nullptr);            // Remove any texture
        rect.setFillColor(sf::Color::White); // Reset back to White color
    }


}


void BattleshipGame::runGame() {
    if (!initTextures() || !initFont()) {
        return;
    }

    // Create rectangles based on the grid
    createRectangles();

    // Main game loop
    while (window.isOpen()) {
        handleEvents();
        update();
        render();
    }
}