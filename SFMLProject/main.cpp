#include "game.h"

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