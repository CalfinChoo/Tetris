#ifndef GAME_H
#define GAME_H

#include "block.h"
#include <chrono>
#include <random>

#define NUM_TILES_WIDTH 10
#define NUM_TILES_HEIGHT 22
#define BOARD_WIDTH (NUM_TILES_WIDTH * SQUARE_SIZE)
#define BOARD_HEIGHT (NUM_TILES_HEIGHT * SQUARE_SIZE)
#define NUM_VERTICAL_LINES 9
#define NUM_HORIZONTAL_LINES 19

class Game {
public:
    Game(sf::RenderWindow& window, float x = 0, float y = 0);
    ~Game();
    bool getLoss() const;
    void setLoss();
    void detectLoss();
    bool placeBlock();
    void cleanRows();
    bool handleEvent();
    bool update();
    void display();
    
    void doUp();
    void doDown();
    void doLeft();
    void doRight();
    bool doSpace();
    void doC();
private:
    sf::Vector2f origin;
    sf::RenderWindow* window;
    sf::View view;
    sf::Transform transform; // current transformation to apply to block
    sf::Clock clock;
    sf::Event event;
    sf::Vertex verticalLines[NUM_VERTICAL_LINES + 2][2];
    sf::Vertex horizontalLines[NUM_HORIZONTAL_LINES + 2][2];
    sf::Font font;
    std::vector<BlockType> possibleBlocks{ I, J, L, O, S, T, Z };
    std::vector<BlockType> bag;
    std::vector<std::vector<sf::RectangleShape*>> board;
    Block* currentBlock;
    Block* held;
    bool hold;
    bool loss;
    int level;
    int linesCleared;
    int score;
    sf::Text holdText, levelText, scoreText;
    Block* nextQ[3];
};


#endif