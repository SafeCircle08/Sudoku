#pragma once
#include <vector>
#include <SFML/Graphics.hpp>

class SudokuGrid {
private:
    sf::Color GRID_COLOR = sf::Color::White;
    sf::RenderWindow& WINDOW;
    const sf::Font FONT = sf::Font("C:/Users/Utente/AppData/Local/Microsoft/Windows/Fonts/big-shot.ttf");
    const int ROWS = 9;
    const int COLUMNS = 9;
    const int BLOCK_ROWS = 3;
    const int BLOCK_COLS = 3;
    const int PIXELS_BLOCK_SIZE = 60;
    const float GRID_MAX_THICKNESS = 4;
    const float GRID_MIN_THICKNESS = 1;
    const float GRID_LINE_DIM = (float) (PIXELS_BLOCK_SIZE * ROWS);
    float GRID_LINE_X;
    float GRID_LINE_Y;

    std::vector<std::vector<int>> grid;
    int rowIndexSelected = -1;
    int colIndexSelected = -1;

    [[nodiscard]] bool checkRow(int row, int num) const;
    [[nodiscard]] bool checkColumn(int col, int num) const;
    [[nodiscard]] bool checkBlock(int row, int col, int num) const;
    [[nodiscard]] bool isValid(int row, int col, int num) const;
    void fillRow(int rowN);
    void fillColumn(int colN);
    void resetGrid();
    bool solveSudoku();
    void printGrid() const;
    int getBlockPxSize() const;
    void drawVerticalLines(sf::Color col);
    void drawHorizontalLines(sf::Color col);
    void drawGrid(sf::Color col);
    void drawNumbers(const sf::Font& font, sf::Color col);
    int getSelectedRowIndex() const;
    int getSelectedColumnIndex() const;
    void manageMousePosition();

public:
    explicit SudokuGrid(sf::RenderWindow& window);
    void updateGrid(sf::Color col);
};