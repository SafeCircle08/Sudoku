#include <iostream>
#include <random>
#include <algorithm>
#include <SFML/Graphics.hpp>
#include "../header/SudokuGrid.h"
#include "../header/Random.h"
#include  "../header/Mouse.h"

enum FILL_WAYS {
    FILL_IN_COLUMN,
    FILL_IN_ROW,
};

int SudokuGrid::getBlockPxSize() const { return PIXELS_BLOCK_SIZE; }
SudokuGrid::SudokuGrid(sf::RenderWindow& window) : WINDOW(window), grid(9, std::vector<int>(9, 0)) {
    this->GRID_LINE_X = (WINDOW.getSize().x - GRID_LINE_DIM) / 2;
    this->GRID_LINE_Y = (WINDOW.getSize().y - GRID_LINE_DIM) / 2;
    SudokuGrid::GRID_COLOR = sf::Color::White;

    switch (Random::getRandomInt(0, 1)) {
        case FILL_IN_COLUMN: fillColumn(Random::getRandomInt(0, COLUMNS - 1)); break;
        case FILL_IN_ROW: fillRow(Random::getRandomInt(0, ROWS - 1)); break;
        default: break;
    }

    printGrid();

    solveSudoku();

    std::cout << "*****************************" << std::endl;

    printGrid();

}
void SudokuGrid::resetGrid() {
    for (auto &row : grid) std::fill(row.begin(), row.end(), 0);
}
bool SudokuGrid::checkRow(int row, int num) const {
    for (int i = 0; i < ROWS; i++) {
        if (grid[row][i] == num) { return false; }
    }
    return true;
}
bool SudokuGrid::checkColumn(int col, int num) const {
    for (int i = 0; i < COLUMNS; i++) {
        if (grid[i][col] == num) { return false; }
    }
    return true;
}
bool SudokuGrid::checkBlock(int row, int col, int num) const {
    int startRow = row - (row % BLOCK_ROWS);
    int startColumn = col - (col % BLOCK_COLS);

    for (int i = 0; i < BLOCK_ROWS; i++) {
        for (int j = 0; j < BLOCK_COLS; j++) {
            if (grid[startRow + i][startColumn + j] == num) { return false; }
        }
    }
    return true;
}
bool SudokuGrid::isValid(int row, int col, int num) const {
    return checkRow(row, num) && checkColumn(col, num) && checkBlock(row, col, num);
}
void SudokuGrid::fillColumn(int colN) {
    std::random_device rd;
    std::mt19937 gen(rd());
    std::vector<int> rows{1, 2, 3, 4, 5, 6, 7, 8, 9};
    std::shuffle(rows.begin(), rows.end(), gen);

    for (int row = 0; row < ROWS; row++) {
        grid[row][colN] = rows[row];
    }
}
void SudokuGrid::fillRow(int rowN) {
    std::random_device rd;
    std::mt19937 gen(rd());
    std::vector<int> cols{1, 2, 3, 4, 5, 6, 7, 8, 9};
    std::shuffle(cols.begin(), cols.end(), gen);

    for (int col = 0; col < COLUMNS; col++) {
        grid[col][rowN] = cols[col];
    }
}
bool SudokuGrid::solveSudoku() {
    for (int i = 0; i < ROWS; i++) {
        for (int j = 0; j < COLUMNS; j++) {
            if (grid[i][j] == 0) {
                std::vector<int> nums{1, 2, 3, 4, 5, 6, 7, 8, 9};
                std::shuffle(nums.begin(), nums.end(), std::mt19937(std::random_device{}()));

                for (int num : nums) {
                    if (isValid(i, j, num)) {
                        grid[i][j] = num;
                        if (solveSudoku()) { return true; }
                        grid[i][j] = 0;
                    }
                }
                return false;
            }
        }
    }
    return true;
}
void SudokuGrid::printGrid() const {
    for (int i = 0; i < ROWS; i++) {
        for (int j = 0; j < COLUMNS; j++) {
            std::cout << grid[i][j] << " ";
        }
        std::cout << std::endl;
    }
}
void SudokuGrid::drawVerticalLines(sf::Color col) {
    for (int i = 0; i < COLUMNS + 1; i++) {
        float thickness = (i % 3 == 0) ? GRID_MAX_THICKNESS : GRID_MIN_THICKNESS;
        sf::RectangleShape vLine{sf::Vector2f(thickness, GRID_LINE_DIM)};
        vLine.setPosition(sf::Vector2f(GRID_LINE_X + float (PIXELS_BLOCK_SIZE * i), GRID_LINE_Y));
        vLine.setFillColor(col);
        WINDOW.draw(vLine);
    }
}
void SudokuGrid::drawHorizontalLines(sf::Color col) {
    for (int i = 0; i < ROWS + 1; i++) {
        float thickness = (i % 3 == 0) ? GRID_MAX_THICKNESS : GRID_MIN_THICKNESS;
        sf::RectangleShape vLine{sf::Vector2f(GRID_LINE_DIM, thickness)};
        vLine.setPosition(sf::Vector2f(GRID_LINE_X, GRID_LINE_Y + float (PIXELS_BLOCK_SIZE * i)));
        vLine.setFillColor(col);
        WINDOW.draw(vLine);
    }
}
void SudokuGrid::drawNumbers(const sf::Font& font, sf::Color col) {
    int txtSize = 30;
    sf::Text num(font);
    num.setFillColor(sf::Color::White);
    num.setCharacterSize(txtSize);
    for (int row = 0; row < 9; row++) {
        for (int column = 0; column < 9; column++) {
            int actualNum = grid[column][row];
            num.setString(std::to_string(actualNum));

            sf::FloatRect bounds = num.getLocalBounds();

            int xOffset = 8;
            int yOffset = 10;
            float numX = GRID_LINE_X + (PIXELS_BLOCK_SIZE * row) + (bounds.size.x) + xOffset;
            float numY = GRID_LINE_Y + (PIXELS_BLOCK_SIZE * column) + (bounds.size.y) - yOffset;

            num.setPosition(sf::Vector2f(numX, numY));
            WINDOW.draw(num);
        }
    }
}
int SudokuGrid::getSelectedRowIndex() const {
    sf::Vector2i mousePos = sf::Mouse::getPosition(WINDOW);
    int index = (mousePos.x - GRID_LINE_X) / PIXELS_BLOCK_SIZE;
    return index;
}
int SudokuGrid::getSelectedColumnIndex() const {
    sf::Vector2i mousePos = sf::Mouse::getPosition(WINDOW);
    int index = (mousePos.y - GRID_LINE_Y) / PIXELS_BLOCK_SIZE;
    return index;
}
void SudokuGrid::manageMousePosition() {
    rowIndexSelected = getSelectedRowIndex();
    colIndexSelected = getSelectedColumnIndex();
    if (Mouse::mouseLeftClicked()) {
        std::cout << rowIndexSelected << " " << colIndexSelected << std::endl;
    }
}
void SudokuGrid::drawGrid(sf::Color col) {
    drawVerticalLines(col);
    drawHorizontalLines(col);
    drawNumbers(FONT, col);
}
void SudokuGrid::updateGrid(sf::Color col) {
    manageMousePosition();
    drawGrid(col);
}