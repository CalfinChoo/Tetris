#include "block.h"
#include "game.h"

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
                squares[i + 1].setPosition((center.x - 1.5 * SQUARE_SIZE) + i * SQUARE_SIZE, center.y - 1.5 * SQUARE_SIZE);
                squares[i + 1].setFillColor(color);
                ghostSquares.emplace_back(sf::Vector2f(SQUARE_SIZE, SQUARE_SIZE));
                ghostSquares[i + 1].setPosition((center.x - 1.5 * SQUARE_SIZE) + i * SQUARE_SIZE, center.y - 1.5 * SQUARE_SIZE);
                ghostSquares[i + 1].setFillColor(sf::Color(color.r, color.g, color.b, GHOST_OPACITY));
            }
        }
        break;
    case O:
        center = sf::Vector2f(x + origin.x + .5 * SQUARE_SIZE, y + origin.y - .5 * SQUARE_SIZE);
        color = sf::Color(255, 255, 0); // Yellow
        for (size_t i = 0; i < 2; ++i) {
            squares.emplace_back(sf::Vector2f(SQUARE_SIZE, SQUARE_SIZE));
            squares.emplace_back(sf::Vector2f(SQUARE_SIZE, SQUARE_SIZE));
            squares[2 * i].setPosition((center.x - SQUARE_SIZE) + i * SQUARE_SIZE, center.y - SQUARE_SIZE);
            squares[2 * i].setFillColor(color);
            squares[2 * i + 1].setPosition((center.x - SQUARE_SIZE) + i * SQUARE_SIZE, center.y);
            squares[2 * i + 1].setFillColor(color);
            ghostSquares.emplace_back(sf::Vector2f(SQUARE_SIZE, SQUARE_SIZE));
            ghostSquares.emplace_back(sf::Vector2f(SQUARE_SIZE, SQUARE_SIZE));
            ghostSquares[2 * i].setPosition((center.x - SQUARE_SIZE) + i * SQUARE_SIZE, center.y - SQUARE_SIZE);
            ghostSquares[2 * i].setFillColor(sf::Color(color.r, color.g, color.b, GHOST_OPACITY));
            ghostSquares[2 * i + 1].setPosition((center.x - SQUARE_SIZE) + i * SQUARE_SIZE, center.y);
            ghostSquares[2 * i + 1].setFillColor(sf::Color(color.r, color.g, color.b, GHOST_OPACITY));
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

bool Block::canMove(int dir, const sf::Transform& transform) const {
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