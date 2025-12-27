#include <SFML/Graphics.hpp>
#include "../header/SudokuGrid.h"

constexpr int WINDOW_WIDTH = 700;
constexpr int WINDOW_HEIGHT = 700;

int main() {
    sf::RenderWindow window(sf::VideoMode({WINDOW_WIDTH, WINDOW_HEIGHT}), "Window");
    SudokuGrid sudoku(window);

    while (window.isOpen()) {
        while (const std::optional event = window.pollEvent()) {
            if (event->is<sf::Event::Closed>()) {
                window.close();
            }
        }
        window.clear(sf::Color::Black);
        sudoku.updateGrid(sf::Color::White);
        window.display();
    }
    return 0;
}