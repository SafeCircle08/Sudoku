#include <SFML/Graphics.hpp>
#include "../header/SudokuGrid.h"

const unsigned WINDOW_WIDTH = SudokuGrid::getBlockPxSize() * SudokuGrid::getColumns() + 200;
const unsigned WINDOW_HEIGHT = SudokuGrid::getBlockPxSize() * SudokuGrid::getRows() + 200;

int main() {
    sf::RenderWindow window(sf::VideoMode({WINDOW_WIDTH, WINDOW_HEIGHT}), "Window");
    SudokuGrid sudoku(IN_ASS_PAIN, window,
        sf::Color::Black,
        COLOR_SOFT_LIGHT_BLUE,
        COLOR_SOFT_LIGHT_PURPLE,
        COLOR_MEDIUM_BLUE
    );

    while (window.isOpen()) {
        while (const std::optional event = window.pollEvent()) {
            if (event->is<sf::Event::Closed>()) {
                window.close();
            }
        }
        window.clear(sf::Color::White);

        sudoku.updateGrid(COLOR_CRAZY_LIGHT_BLUE);

        window.display();
    }
    return 0;
}