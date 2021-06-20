#include <SFML/Graphics.hpp>
#include <string>
#include <iostream>
#include <chrono>
#include <algorithm>
#include <vector>
#include <math.h>
#include <random>

#ifndef CONSTANTS_H
#define SQUARE_SIZE 30
#define NUM_TILES_WIDTH 10
#define NUM_TILES_HEIGHT 22
#define BOARD_WIDTH (NUM_TILES_WIDTH * SQUARE_SIZE)
#define BOARD_HEIGHT (NUM_TILES_HEIGHT * SQUARE_SIZE)
#define NUM_VERTICAL_LINES 9
#define NUM_HORIZONTAL_LINES 19
#define GHOST_OPACITY 120
#endif //CONSTANTS_H

//const int SQUARE_SIZE = 30;
//const int NUM_TILES_WIDTH = 10;
//const int NUM_TILES_HEIGHT = 22;
//const int BOARD_WIDTH = NUM_TILES_WIDTH * SQUARE_SIZE;
//const int BOARD_HEIGHT = NUM_TILES_HEIGHT * SQUARE_SIZE;
//const int NUM_VERTICAL_LINES = 10;
//const int NUM_HORIZONTAL_LINES = 19;

const enum BlockType {
    I,
    J,
    L,
    O,
    S,
    T,
    Z
};

class Block {
public:
    Block(BlockType b, std::vector<std::vector<sf::RectangleShape*>>* board, const sf::Vector2f& origin = sf::Vector2f(0, 0), 
        float centerX = 4.5 * SQUARE_SIZE, float centerY = 3.5 * SQUARE_SIZE) throw (int);
    void setSchematic(BlockType t, float x, float y);
    void setOrientation(int o);
    void draw(sf::RenderWindow& window, const sf::Transform& transform) const;
    void updateBoard(std::vector<std::vector<sf::RectangleShape*>>& board, const sf::Transform& transform) const;
    void updateGhost(const sf::Transform& transform);
    sf::Vector2f getCenter() const;
    int getOrientation() const;
    sf::Vector2i getBoardPosition(float x, float y, const sf::Transform& transform) const;
    BlockType getBlockType() const;
    bool canMove(int dir, const sf::Transform& transform) const;
    bool canLower(const sf::Transform& transform, bool ghost = false) const;
    bool canRotate(const sf::Transform& transform) const;
    bool move(int dir, const sf::Transform& transform);
    bool lower(const sf::Transform& transform, bool ghost = false);
private:
    sf::Vector2f origin;
    sf::Vector2f center;
    std::vector<sf::RectangleShape> squares;
    std::vector<sf::RectangleShape> ghostSquares;
    std::vector<std::vector<sf::RectangleShape*>>* board;
    int orientation;
    BlockType blocktype;
    sf::Color color;
};

Block::Block(BlockType b, std::vector<std::vector<sf::RectangleShape*>>* board, const sf::Vector2f& origin, float centerX, float centerY) 
    : board(board), origin(origin), orientation(0), blocktype(b) {
    setSchematic(b, centerX, centerY);
}

bool Block::move(int dir, const sf::Transform& transform) {
    if (canMove(dir, transform)) {
        switch (orientation) {
        case 0:
            center.x += dir * SQUARE_SIZE;
            for (sf::RectangleShape& s : squares) {
                s.move(float(dir * SQUARE_SIZE), 0);
            }
            break;
        case 1:
            center.y -= dir * SQUARE_SIZE;
            for (sf::RectangleShape& s : squares) {
                s.move(0, float(dir * -SQUARE_SIZE));
            }
            break;
        case 2:
            center.x -= dir * SQUARE_SIZE;
            for (sf::RectangleShape& s : squares) {
                s.move(float(dir * -SQUARE_SIZE), 0);
            }
            break;
        case 3:
            center.y += dir * SQUARE_SIZE;
            for (sf::RectangleShape& s : squares) {
                s.move(0, float(dir * SQUARE_SIZE));
            }
            break;
        }
        return true;
    }
    return false;
}

void Block::setSchematic(BlockType t, float x, float y) {
    center = sf::Vector2f(x + origin.x, y + origin.y);
    switch (t) {
    case I:
        center = sf::Vector2f(x + origin.x + .5 * SQUARE_SIZE, y + origin.y + .5 * SQUARE_SIZE);
        color = sf::Color(0, 255, 255); // Cyan
        for (size_t i = 0; i < 4; ++i) {
            squares.emplace_back(sf::Vector2f(SQUARE_SIZE, SQUARE_SIZE));
            squares[i].setPosition((center.x - 2 * SQUARE_SIZE) + i * SQUARE_SIZE, center.y - SQUARE_SIZE);
            squares[i].setFillColor(color);
            ghostSquares.emplace_back(sf::Vector2f(SQUARE_SIZE, SQUARE_SIZE));
            ghostSquares[i].setPosition((center.x - 2 * SQUARE_SIZE) + i * SQUARE_SIZE, center.y - SQUARE_SIZE);
            ghostSquares[i].setFillColor(sf::Color(color.r, color.g, color.b, GHOST_OPACITY));
        }
        break;
    case J:
        color = sf::Color(0, 0, 255); // Blue
        for (size_t i = 0; i < 4; ++i) {
            if (i < 3) {
                squares.emplace_back(sf::Vector2f(SQUARE_SIZE, SQUARE_SIZE));
                squares[i].setPosition((center.x - 1.5 * SQUARE_SIZE) + i * SQUARE_SIZE, center.y - .5 * SQUARE_SIZE);
                squares[i].setFillColor(color); 
                ghostSquares.emplace_back(sf::Vector2f(SQUARE_SIZE, SQUARE_SIZE));
                ghostSquares[i].setPosition((center.x - 1.5 * SQUARE_SIZE) + i * SQUARE_SIZE, center.y - .5 * SQUARE_SIZE);
                ghostSquares[i].setFillColor(sf::Color(color.r, color.g, color.b, GHOST_OPACITY));
            }           
            else {
                squares.emplace_back(sf::Vector2f(SQUARE_SIZE, SQUARE_SIZE));
                squares[i].setPosition(center.x - 1.5 * SQUARE_SIZE, center.y - 1.5 * SQUARE_SIZE);
                squares[i].setFillColor(color);
                ghostSquares.emplace_back(sf::Vector2f(SQUARE_SIZE, SQUARE_SIZE));
                ghostSquares[i].setPosition(center.x - 1.5 * SQUARE_SIZE, center.y - 1.5 * SQUARE_SIZE);
                ghostSquares[i].setFillColor(sf::Color(color.r, color.g, color.b, GHOST_OPACITY));
            }
        }
        break;
    case L:
        color = sf::Color(255, 128, 0); // Orange
        for (size_t i = 0; i < 3; ++i) {
            squares.emplace_back(sf::Vector2f(SQUARE_SIZE, SQUARE_SIZE));
            squares[i].setPosition((center.x - 1.5 * SQUARE_SIZE) + i * SQUARE_SIZE, center.y - .5 * SQUARE_SIZE);
            squares[i].setFillColor(color);
            ghostSquares.emplace_back(sf::Vector2f(SQUARE_SIZE, SQUARE_SIZE));
            ghostSquares[i].setPosition((center.x - 1.5 * SQUARE_SIZE) + i * SQUARE_SIZE, center.y - .5 * SQUARE_SIZE);
            ghostSquares[i].setFillColor(sf::Color(color.r, color.g, color.b, GHOST_OPACITY));
            if (i == 2) {
                squares.emplace_back(sf::Vector2f(SQUARE_SIZE, SQUARE_SIZE));
                squares[i+1].setPosition((center.x - 1.5 * SQUARE_SIZE) + i * SQUARE_SIZE, center.y - 1.5 * SQUARE_SIZE);
                squares[i+1].setFillColor(color);
                ghostSquares.emplace_back(sf::Vector2f(SQUARE_SIZE, SQUARE_SIZE));
                ghostSquares[i+1].setPosition((center.x - 1.5 * SQUARE_SIZE) + i * SQUARE_SIZE, center.y - 1.5 * SQUARE_SIZE);
                ghostSquares[i+1].setFillColor(sf::Color(color.r, color.g, color.b, GHOST_OPACITY));
            }
        }
        break;
    case O:
        center = sf::Vector2f(x + origin.x + .5 * SQUARE_SIZE, y + origin.y - .5 * SQUARE_SIZE);
        color = sf::Color(255, 255, 0); // Yellow
        for (size_t i = 0; i < 2; ++i) {
            squares.emplace_back(sf::Vector2f(SQUARE_SIZE, SQUARE_SIZE));
            squares.emplace_back(sf::Vector2f(SQUARE_SIZE, SQUARE_SIZE));
            squares[2*i].setPosition((center.x - SQUARE_SIZE) + i * SQUARE_SIZE, center.y - SQUARE_SIZE);
            squares[2*i].setFillColor(color); 
            squares[2*i+1].setPosition((center.x - SQUARE_SIZE) + i * SQUARE_SIZE, center.y);
            squares[2*i+1].setFillColor(color);
            ghostSquares.emplace_back(sf::Vector2f(SQUARE_SIZE, SQUARE_SIZE));
            ghostSquares.emplace_back(sf::Vector2f(SQUARE_SIZE, SQUARE_SIZE));
            ghostSquares[2*i].setPosition((center.x - SQUARE_SIZE) + i * SQUARE_SIZE, center.y - SQUARE_SIZE);
            ghostSquares[2*i].setFillColor(sf::Color(color.r, color.g, color.b, GHOST_OPACITY));
            ghostSquares[2*i+1].setPosition((center.x - SQUARE_SIZE) + i * SQUARE_SIZE, center.y);
            ghostSquares[2*i+1].setFillColor(sf::Color(color.r, color.g, color.b, GHOST_OPACITY));
        }
        break;
    case S:
        color = sf::Color(0, 255, 0); // Green
        for (size_t i = 0; i < 4; ++i) {
            if (i < 2) {
                squares.emplace_back(sf::Vector2f(SQUARE_SIZE, SQUARE_SIZE));
                squares[i].setPosition((center.x - 1.5 * SQUARE_SIZE) + i * SQUARE_SIZE, center.y - .5 * SQUARE_SIZE);
                squares[i].setFillColor(color);
                ghostSquares.emplace_back(sf::Vector2f(SQUARE_SIZE, SQUARE_SIZE));
                ghostSquares[i].setPosition((center.x - 1.5 * SQUARE_SIZE) + i * SQUARE_SIZE, center.y - .5 * SQUARE_SIZE);
                ghostSquares[i].setFillColor(sf::Color(color.r, color.g, color.b, GHOST_OPACITY));
            }
            else {
                squares.emplace_back(sf::Vector2f(SQUARE_SIZE, SQUARE_SIZE));
                squares[i].setPosition((center.x - 1.5 * SQUARE_SIZE) + (i - 1) * SQUARE_SIZE, center.y - 1.5 * SQUARE_SIZE);
                squares[i].setFillColor(color);
                ghostSquares.emplace_back(sf::Vector2f(SQUARE_SIZE, SQUARE_SIZE));
                ghostSquares[i].setPosition((center.x - 1.5 * SQUARE_SIZE) + (i - 1) * SQUARE_SIZE, center.y - 1.5 * SQUARE_SIZE);
                ghostSquares[i].setFillColor(sf::Color(color.r, color.g, color.b, GHOST_OPACITY));
            }
        }
        break;
    case T:
        color = sf::Color(255, 0, 255); // Magenta
        for (size_t i = 0; i < 4; ++i) {
            if (i < 3) {
                squares.emplace_back(sf::Vector2f(SQUARE_SIZE, SQUARE_SIZE));
                squares[i].setPosition((center.x - 1.5 * SQUARE_SIZE) + i * SQUARE_SIZE, center.y - .5 * SQUARE_SIZE);
                squares[i].setFillColor(color);
                ghostSquares.emplace_back(sf::Vector2f(SQUARE_SIZE, SQUARE_SIZE));
                ghostSquares[i].setPosition((center.x - 1.5 * SQUARE_SIZE) + i * SQUARE_SIZE, center.y - .5 * SQUARE_SIZE);
                ghostSquares[i].setFillColor(sf::Color(color.r, color.g, color.b, GHOST_OPACITY));
            }
            else {
                squares.emplace_back(sf::Vector2f(SQUARE_SIZE, SQUARE_SIZE));
                squares[i].setPosition(center.x - .5 * SQUARE_SIZE, center.y - 1.5 * SQUARE_SIZE);
                squares[i].setFillColor(color);
                ghostSquares.emplace_back(sf::Vector2f(SQUARE_SIZE, SQUARE_SIZE));
                ghostSquares[i].setPosition(center.x - .5 * SQUARE_SIZE, center.y - 1.5 * SQUARE_SIZE);
                ghostSquares[i].setFillColor(sf::Color(color.r, color.g, color.b, GHOST_OPACITY));
            }
        }
        break;
    case Z:
        color = sf::Color(255, 0, 0); // Red
        for (size_t i = 0; i < 4; ++i) {
            if (i < 2) {
                squares.emplace_back(sf::Vector2f(SQUARE_SIZE, SQUARE_SIZE));
                squares[i].setPosition((center.x - 1.5 * SQUARE_SIZE) + i * SQUARE_SIZE, center.y - 1.5 * SQUARE_SIZE);
                squares[i].setFillColor(color); 
                ghostSquares.emplace_back(sf::Vector2f(SQUARE_SIZE, SQUARE_SIZE));
                ghostSquares[i].setPosition((center.x - 1.5 * SQUARE_SIZE) + i * SQUARE_SIZE, center.y - 1.5 * SQUARE_SIZE);
                ghostSquares[i].setFillColor(sf::Color(color.r, color.g, color.b, GHOST_OPACITY));
            }
            else {
                squares.emplace_back(sf::Vector2f(SQUARE_SIZE, SQUARE_SIZE));
                squares[i].setPosition((center.x - 1.5 * SQUARE_SIZE) + (i - 1) * SQUARE_SIZE, center.y - .5 * SQUARE_SIZE);
                squares[i].setFillColor(color);
                ghostSquares.emplace_back(sf::Vector2f(SQUARE_SIZE, SQUARE_SIZE));
                ghostSquares[i].setPosition((center.x - 1.5 * SQUARE_SIZE) + (i - 1) * SQUARE_SIZE, center.y - .5 * SQUARE_SIZE);
                ghostSquares[i].setFillColor(sf::Color(color.r, color.g, color.b, GHOST_OPACITY));
            }
        }
        break;
    }
    while (true) { //check for spawn collision
        bool endLoop = true;
        for (sf::RectangleShape& s : squares) {
            sf::Vector2i boardPosition = getBoardPosition(s.getPosition().x, s.getPosition().y, sf::Transform());
            if (boardPosition.y < 0 || boardPosition.y >= NUM_TILES_HEIGHT) { // couldn't spawn block
                throw 0;
            }
            if (boardPosition.x >= 0 && boardPosition.x < NUM_TILES_WIDTH && (*board)[boardPosition.y][boardPosition.x] != nullptr) {
                center.y -= SQUARE_SIZE;
                for (sf::RectangleShape& s : squares) {
                    s.move(0, -SQUARE_SIZE);
                }
                endLoop = false;
                break;
            }
        }
        if (endLoop) break;
    }
}

void Block::setOrientation(int o) {
    orientation = o;
}

void Block::draw(sf::RenderWindow& window, const sf::Transform& transform) const {
    for (size_t i = 0; i < ghostSquares.size(); ++i) {
        window.draw(ghostSquares[i], transform);
    }
    for (size_t i = 0; i < squares.size(); ++i) {
        window.draw(squares[i], transform);
    }
}

void Block::updateBoard(std::vector<std::vector<sf::RectangleShape*>>& board, const sf::Transform& transform) const {
    for (const sf::RectangleShape& s : squares) {
        sf::Vector2f transformedPoint = transform.transformPoint(s.getPosition().x, s.getPosition().y);
        transformedPoint.x = round(transformedPoint.x);
        transformedPoint.y = round(transformedPoint.y);
        if (orientation == 1 || orientation == 2) { //hella jank
            transformedPoint.x -= SQUARE_SIZE;
        }
        if (orientation == 2 || orientation == 3) {
            transformedPoint.y -= SQUARE_SIZE;
        }
        int x = (transformedPoint.x - origin.x) / SQUARE_SIZE;
        int y = NUM_TILES_HEIGHT - (transformedPoint.y - origin.y + SQUARE_SIZE) / SQUARE_SIZE;
        board[y][x] = new sf::RectangleShape(sf::Vector2f(SQUARE_SIZE, SQUARE_SIZE));
        board[y][x]->setPosition(transformedPoint.x, transformedPoint.y);
        board[y][x]->setFillColor(s.getFillColor());
    }
}

void Block::updateGhost(const sf::Transform& transform) {
    for (size_t i = 0; i < squares.size(); ++i) {
        ghostSquares[i].setPosition(squares[i].getPosition());
    }
    while (lower(transform, true)) {}
}

sf::Vector2f Block::getCenter() const {
    return center;
}

int Block::getOrientation() const {
    return orientation;
}

sf::Vector2i Block::getBoardPosition(float x, float y, const sf::Transform& transform) const {
    sf::Vector2f transformedPoint = transform.transformPoint(x, y);
    transformedPoint.x = round(transformedPoint.x);
    transformedPoint.y = round(transformedPoint.y);
    if (orientation == 1 || orientation == 2) { //hella jank
        transformedPoint.x -= SQUARE_SIZE;
    }
    if (orientation == 2 || orientation == 3) {
        transformedPoint.y -= SQUARE_SIZE;
    }
    return sf::Vector2i((transformedPoint.x - origin.x) / SQUARE_SIZE, NUM_TILES_HEIGHT - (transformedPoint.y - origin.y + SQUARE_SIZE) / SQUARE_SIZE);
}

BlockType Block::getBlockType() const {
    return blocktype;
}

bool Block::canMove(int dir, const sf::Transform & transform) const {
    for (const sf::RectangleShape& s : squares) {
        sf::Vector2i boardPosition = getBoardPosition(s.getPosition().x, s.getPosition().y, transform);
        if (boardPosition.x + dir < 0 || boardPosition.x + dir >= NUM_TILES_WIDTH || (*board)[boardPosition.y][uint64_t(boardPosition.x) + dir] != nullptr)
            return false;
    }
    return true;
}

bool Block::canLower(const sf::Transform& transform, bool ghost) const {
    if (!ghost) {
        for (const sf::RectangleShape& s : squares) {
            sf::Vector2i boardPosition = getBoardPosition(s.getPosition().x, s.getPosition().y, transform);
            if (boardPosition.y <= 0 || boardPosition.y >= NUM_TILES_HEIGHT || (*board)[uint64_t(boardPosition.y) - 1][boardPosition.x] != nullptr) {
                return false;
            }
        }
    }
    else {
        for (const sf::RectangleShape& s : ghostSquares) {
            sf::Vector2i boardPosition = getBoardPosition(s.getPosition().x, s.getPosition().y, transform);
            if (boardPosition.y <= 0 || boardPosition.y >= NUM_TILES_HEIGHT || (*board)[uint64_t(boardPosition.y) - 1][boardPosition.x] != nullptr) {
                return false;
            }
        }
    }
    return true;
}

bool Block::canRotate(const sf::Transform& transform) const {
    if (blocktype == O) return false;
    for (const sf::RectangleShape& s : squares) {
        sf::Vector2i boardPosition = getBoardPosition(s.getPosition().x, s.getPosition().y, transform);
        if (orientation == 2) boardPosition.x += 1; // more jank stuff
        else if (orientation == 0) boardPosition.x -= 1;
        if (boardPosition.y <= 0 || boardPosition.y >= NUM_TILES_HEIGHT || boardPosition.x < 0 || boardPosition.x >= NUM_TILES_WIDTH || (*board)[boardPosition.y][boardPosition.x] != nullptr) {
            std::cout << boardPosition.y << std::endl;
            return false;
        }
    }
    return true;
}

bool Block::lower(const sf::Transform& transform, bool ghost) {
    if (!ghost) {
        if (canLower(transform)) {
            switch (orientation) {
            case 0:
                center.y += SQUARE_SIZE;
                for (sf::RectangleShape& s : squares) {
                    s.move(0, SQUARE_SIZE);
                }
                break;
            case 1:
                center.x += SQUARE_SIZE;
                for (sf::RectangleShape& s : squares) {
                    s.move(SQUARE_SIZE, 0);
                }
                break;
            case 2:
                center.y -= SQUARE_SIZE;
                for (sf::RectangleShape& s : squares) {
                    s.move(0, -SQUARE_SIZE);
                }
                break;
            case 3:
                center.x -= SQUARE_SIZE;
                for (sf::RectangleShape& s : squares) {
                    s.move(-SQUARE_SIZE, 0);
                }
                break;
            }
            return true;
        }
    }
    else {
        if (canLower(transform, ghost)) {
            switch (orientation) {
            case 0:
                for (sf::RectangleShape& s : ghostSquares) {
                    s.move(0, SQUARE_SIZE);
                }
                break;
            case 1:
                for (sf::RectangleShape& s : ghostSquares) {
                    s.move(SQUARE_SIZE, 0);
                }
                break;
            case 2:
                for (sf::RectangleShape& s : ghostSquares) {
                    s.move(0, -SQUARE_SIZE);
                }
                break;
            case 3:
                for (sf::RectangleShape& s : ghostSquares) {
                    s.move(-SQUARE_SIZE, 0);
                }
                break;
            }
            return true;
        }
    }
    return false;
}

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
private:
    sf::Vector2f origin;
    sf::RenderWindow* window;
    sf::View view;
    sf::Transform transform; // current transformation to apply to block
    sf::Clock clock;
    sf::Event event;
    sf::Vertex verticalLines[NUM_VERTICAL_LINES+2][2];
    sf::Vertex horizontalLines[NUM_HORIZONTAL_LINES+2][2];
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

Game::Game(sf::RenderWindow& window, float x, float y) : origin(sf::Vector2f(x, y)), window(&window), 
                               view(sf::FloatRect(0, 2 * SQUARE_SIZE, 2 * BOARD_WIDTH, BOARD_HEIGHT - 2 * SQUARE_SIZE)),
                               bag(possibleBlocks), held(nullptr), hold(true), loss(false), level(1), linesCleared(0), score(0) {

    std::vector<std::vector<sf::RectangleShape*>> init(NUM_TILES_HEIGHT, std::vector<sf::RectangleShape*>(NUM_TILES_WIDTH, nullptr));
    board = init;

    if (!font.loadFromFile("arial_narrow_7.ttf"))
    {
        std::cout << "Font file \'arial_narrow_7.ttf\' not found" << std::endl;
        exit(1);
    }
    else {
        holdText.setString("Hold");
        holdText.setFont(font);
        holdText.setCharacterSize(SQUARE_SIZE);
        holdText.setPosition(origin.x + 1.5 * BOARD_WIDTH - SQUARE_SIZE, origin.y + 3 * SQUARE_SIZE);
        levelText.setString("Level : 1");
        levelText.setFont(font);
        levelText.setCharacterSize(SQUARE_SIZE);
        levelText.setPosition(origin.x + 1.5 * BOARD_WIDTH - 1.75 * SQUARE_SIZE, origin.y + 7 * SQUARE_SIZE);
        scoreText.setString("Score : 0");
        scoreText.setFont(font);
        scoreText.setCharacterSize(SQUARE_SIZE);
        scoreText.setPosition(origin.x + 1.5 * BOARD_WIDTH - 1.75 * SQUARE_SIZE, origin.y + 8.5 * SQUARE_SIZE);
    }

    unsigned seed = std::chrono::system_clock::now().time_since_epoch().count();
    std::shuffle(bag.begin(), bag.end(), std::default_random_engine(seed));
    currentBlock = new Block(bag.back(), &board, origin);
    currentBlock->updateGhost(transform);
    bag.pop_back();
    for (size_t i = 0; i < 3; ++i) {
        nextQ[i] = new Block(bag.back(), &board, origin, 1.5 * BOARD_WIDTH, 12 * SQUARE_SIZE + 3 * SQUARE_SIZE * i);
        bag.pop_back();
    }

    for (size_t i = 0; i < NUM_VERTICAL_LINES + 2; ++i) {
        verticalLines[i][0].position = sf::Vector2f(origin.x + SQUARE_SIZE * i, origin.y + 2 * SQUARE_SIZE);
        verticalLines[i][0].color = sf::Color(255, 255, 255, 100);
        verticalLines[i][1].position = sf::Vector2f(origin.x + SQUARE_SIZE * i, origin.y + SQUARE_SIZE * NUM_TILES_HEIGHT);
        verticalLines[i][1].color = sf::Color(255, 255, 255, 100);
    }
    for (size_t i = 0; i < NUM_HORIZONTAL_LINES + 2; ++i) {
        horizontalLines[i][0].position = sf::Vector2f(origin.x, origin.y + 2 * SQUARE_SIZE + SQUARE_SIZE * i);
        horizontalLines[i][0].color = sf::Color(255, 255, 255, 100);
        horizontalLines[i][1].position = sf::Vector2f(origin.x + SQUARE_SIZE * NUM_TILES_WIDTH, origin.y + 2 * SQUARE_SIZE + SQUARE_SIZE * i);
        horizontalLines[i][1].color = sf::Color(255, 255, 255, 100);
    }

    this->window->setView(this->view);
}

Game::~Game() {
    delete held;
    for (size_t y = 0; y < NUM_TILES_HEIGHT; ++y) {
        for (size_t x = 0; x < NUM_TILES_WIDTH; ++x) {
            delete board[y][x];
        }
    }
    for (size_t i = 0; i < 3; ++i) {
        delete nextQ[i];
    }
}

bool Game::getLoss() const {
    return loss;
}

void Game::setLoss() {
    loss = true;
}

void Game::detectLoss() {
    for (size_t i = NUM_TILES_HEIGHT - 2; i < NUM_TILES_HEIGHT; ++i) {
        for (sf::RectangleShape* s : board[i]) {
            if (s != nullptr) {
                loss = true;
                delete currentBlock;
                return;
            }
        }
    }
}

bool Game::placeBlock() {
    currentBlock->updateBoard(board, transform);
    cleanRows();
    transform = sf::Transform();
    if (bag.empty()) {
        bag = possibleBlocks;
        std::random_shuffle(bag.begin(), bag.end());
    }
    delete currentBlock;
    try {
        currentBlock = new Block(nextQ[0]->getBlockType(), &board, origin);
        currentBlock->updateGhost(transform);
        delete nextQ[0];
    }
    catch (int n) {
        loss = true;
        return false;
    }
    if (loss) {
        window->close();
    }
    nextQ[0] = new Block(nextQ[1]->getBlockType(), &board, origin, 1.5 * BOARD_WIDTH, 12 * SQUARE_SIZE);
    delete nextQ[1];
    nextQ[1] = new Block(nextQ[2]->getBlockType(), &board, origin, 1.5 * BOARD_WIDTH, 12 * SQUARE_SIZE + 3 * SQUARE_SIZE);
    delete nextQ[2];
    nextQ[2] = new Block(bag.back(), &board, origin, 1.5 * BOARD_WIDTH, 12 * SQUARE_SIZE + 3 * SQUARE_SIZE * 2);
    bag.pop_back();

    hold = true;
    clock.restart();
    return true;
}

void Game::cleanRows() {
    int oustPos = 0, countOust = 0;
    for (int y = 0; y < NUM_TILES_HEIGHT; ++y) {
        bool oust = true;
        for (size_t x = 0; x < NUM_TILES_WIDTH; ++x) {
            if (board[y][x] == nullptr) {
                oust = false;
                break;
            }
        }
        if (oust) {
            ++countOust;
        }
        else {
            for (sf::RectangleShape* s : board[y]) {
                if (s != nullptr) s->move(0, (y - oustPos) * SQUARE_SIZE);
            }
            if (y != oustPos) board[y].swap(board[oustPos]);
            ++oustPos;
        }
    }
    for (size_t i = 0; i < countOust; ++i) {
        for (sf::RectangleShape* s : board.back()) delete s;
        board.pop_back();
    }
    for (size_t i = 0; i < countOust; ++i) {
        board.push_back(std::vector<sf::RectangleShape*>(NUM_TILES_WIDTH, nullptr));
    }
    switch (countOust) { // row clear point calculation
    case 1:
        score += 100 * level;
        break;
    case 2:
        score += 300 * level;
        break;
    case 3:
        score += 500 * level;
        break;
    case 4:
        score += 800 * level;
        break;
    }
    linesCleared += countOust;
    level = (linesCleared / 10) + 1;
    levelText.setString("Level : " + std::to_string(level));
    scoreText.setString("Score : " + std::to_string(score));
}

bool Game::handleEvent() { 
    while (window->pollEvent(event)) {
        switch (event.type) {
        case sf::Event::Closed:
            window->close();
            break;
        case sf::Event::KeyPressed:
            if (sf::Keyboard::isKeyPressed(sf::Keyboard::Up)) {
                sf::Transform oldState = transform;
                transform.rotate(90, currentBlock->getCenter());
                if (!currentBlock->canRotate(transform)) {
                    transform = oldState;
                }
                else {
                    currentBlock->setOrientation((currentBlock->getOrientation() + 1) % 4);
                }
                currentBlock->updateGhost(transform);
            }
            else if (sf::Keyboard::isKeyPressed(sf::Keyboard::Down)) {
                if (currentBlock->lower(transform)) score += 1;
                scoreText.setString("Score : " + std::to_string(score));
            }
            else if (sf::Keyboard::isKeyPressed(sf::Keyboard::Left)) {
                currentBlock->move(-1, transform);
                currentBlock->updateGhost(transform);
            }
            else if (sf::Keyboard::isKeyPressed(sf::Keyboard::Right)) {
                currentBlock->move(1, transform);
                currentBlock->updateGhost(transform);
            }
            else if (sf::Keyboard::isKeyPressed(sf::Keyboard::Space)) {
                while (currentBlock->lower(transform)) {
                    score += 2;
                }
                scoreText.setString("Score : " + std::to_string(score));
                if (!placeBlock()) return false;
            }
            else if (sf::Keyboard::isKeyPressed(sf::Keyboard::C) && hold) {
                if (held == nullptr) {
                    held = new Block(currentBlock->getBlockType(), &board, origin, 
                        (currentBlock->getBlockType() == I || currentBlock->getBlockType() == O) ? 1.5 * BOARD_WIDTH - .5 * SQUARE_SIZE : 1.5 * BOARD_WIDTH, 6 * SQUARE_SIZE);
                    delete currentBlock;
                    if (bag.empty()) {
                        bag = possibleBlocks;
                        std::random_shuffle(bag.begin(), bag.end());
                    }
                    currentBlock = new Block(nextQ[0]->getBlockType(), &board, origin);
                    transform = sf::Transform();
                    currentBlock->updateGhost(transform);
                    delete nextQ[0];
                    nextQ[0] = new Block(nextQ[1]->getBlockType(), &board, origin, 1.5 * BOARD_WIDTH, 12 * SQUARE_SIZE);
                    delete nextQ[1];
                    nextQ[1] = new Block(nextQ[2]->getBlockType(), &board, origin, 1.5 * BOARD_WIDTH, 12 * SQUARE_SIZE + 3 * SQUARE_SIZE);
                    delete nextQ[2];
                    nextQ[2] = new Block(bag.back(), &board, origin, 1.5 * BOARD_WIDTH, 12 * SQUARE_SIZE + 3 * SQUARE_SIZE * 2);
                    bag.pop_back();
                }
                else {
                    hold = false;
                    BlockType swap = currentBlock->getBlockType();
                    delete currentBlock;
                    currentBlock = new Block(held->getBlockType(), &board, origin);
                    transform = sf::Transform();
                    currentBlock->updateGhost(transform);
                    delete held;
                    held = new Block(swap, &board, origin,
                        (swap == I || swap == O) ? 1.5 * BOARD_WIDTH - .5 * SQUARE_SIZE : 1.5 * BOARD_WIDTH, 6 * SQUARE_SIZE);
                }
            }
            break;
        }
    }
    return true;
}

bool Game::update() {
    if (!handleEvent()) return false;
    float elapsed = clock.getElapsedTime().asSeconds();
    float timePerRow = pow((.8 - (level - 1.0) * .007), (level - 1.0));
    if (elapsed > timePerRow) { 
        if (!currentBlock->lower(transform)) {
            if (elapsed > timePerRow + .5) {
                if (!placeBlock()) return false;
            }
        }
        else {
            clock.restart();
        }
    }
    detectLoss();
    return true;
}

void Game::display() {
    window->clear();
    for (size_t i = 0; i < NUM_VERTICAL_LINES+2; ++i) {
        window->draw(verticalLines[i], 2, sf::Lines);
    }
    for (size_t i = 0; i < NUM_HORIZONTAL_LINES+2; ++i) {
        window->draw(horizontalLines[i], 2, sf::Lines);
    }
    if (held != nullptr) held->draw(*window, sf::Transform());
    currentBlock->draw(*window, transform);
    window->draw(holdText);
    window->draw(levelText);
    window->draw(scoreText);
    for (size_t i = 0; i < 3; ++i) {
        nextQ[i]->draw(*window, sf::Transform());
    }
    for (size_t y = 0; y < NUM_TILES_HEIGHT; ++y) {
        for (size_t x = 0; x < NUM_TILES_WIDTH; ++x) {
            if (board[y][x] != nullptr) window->draw(*board[y][x]);
        }
    }
    window->display();
}

int main()
{
    sf::RenderWindow window(sf::VideoMode(2 * BOARD_WIDTH, BOARD_HEIGHT - 2 * SQUARE_SIZE), "Testris");
    Game game(window);
    while (window.isOpen() && !game.getLoss()) {
        if (game.update()) {
            game.display();
        }
    }
    if (game.getLoss()) std::cout << "You Lose" << std::endl;
    return 0;
}