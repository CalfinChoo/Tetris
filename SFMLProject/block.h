#ifndef BLOCK_H
#define BLOCK_H

#include <SFML/Graphics.hpp>
#include <iostream>

#define SQUARE_SIZE 30
#define GHOST_OPACITY 120

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

#endif