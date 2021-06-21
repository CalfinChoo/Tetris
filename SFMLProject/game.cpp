#include "game.h"

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
                doUp();
            }
            else if (sf::Keyboard::isKeyPressed(sf::Keyboard::Down)) {
                doDown();
            }
            else if (sf::Keyboard::isKeyPressed(sf::Keyboard::Left)) {
                doLeft();
            }
            else if (sf::Keyboard::isKeyPressed(sf::Keyboard::Right)) {
                doRight();
            }
            else if (sf::Keyboard::isKeyPressed(sf::Keyboard::Space)) {
                if (!doSpace()) return false;
            }
            else if (sf::Keyboard::isKeyPressed(sf::Keyboard::C) && hold) {
                doC();
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
    for (size_t i = 0; i < NUM_VERTICAL_LINES + 2; ++i) {
        window->draw(verticalLines[i], 2, sf::Lines);
    }
    for (size_t i = 0; i < NUM_HORIZONTAL_LINES + 2; ++i) {
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

void Game::doUp() {
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

void Game::doDown() {
    if (currentBlock->lower(transform)) score += 1;
    scoreText.setString("Score : " + std::to_string(score));
}

void Game::doLeft() {
    currentBlock->move(-1, transform);
    currentBlock->updateGhost(transform);
}

void Game::doRight() {
    currentBlock->move(1, transform);
    currentBlock->updateGhost(transform);
}

bool Game::doSpace() {
    while (currentBlock->lower(transform)) {
        score += 2;
    }
    scoreText.setString("Score : " + std::to_string(score));
    if (!placeBlock()) return false;
    return true;
}

void Game::doC() {
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