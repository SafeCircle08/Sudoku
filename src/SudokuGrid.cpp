#include <iostream>
#include <random>
#include <algorithm>
#include <SFML/Graphics.hpp>
#include "../header/SudokuGrid.h"
#include "../header/Random.h"
#include  "../header/Mouse.h"

SudokuGrid::SudokuGrid(DIFFICULTY difficulty, sf::RenderWindow& window,
    sf::Color gridColor, sf::Color hlColor,
    sf::Color hlNumColor, sf::Color innerLinesCol) :
    WINDOW(window), grid(9, std::vector<int>(9, 0)), difficulty(difficulty),
    GRID_COLOR(gridColor), GRID_HIGHLIGHT_COLOR(hlColor), GRID_HIGHLIGHT_NUMBER_CELL_COLOR(hlNumColor),
    GRID_INNER_LINE_COLOR(innerLinesCol)
{
    this->GRID_LINE_X = (WINDOW.getSize().x - GRID_LINE_DIM) / 2;
    this->GRID_LINE_Y = (WINDOW.getSize().y - GRID_LINE_DIM) / 2 - 80;

    switch (Random::getRandomInt(0, 1)) {
        case FILL_IN_COLUMN: fillColumn(Random::getRandomInt(0, COLUMNS - 1)); break;
        case FILL_IN_ROW: fillRow(Random::getRandomInt(0, ROWS - 1)); break;
        default: break;
    }

    solveSudoku();
    playerGrid = grid;
    hideCells(difficulty);
    drawnGrid = playerGrid;
}

//--------------------GETS--------------------
int SudokuGrid::getColumns() { return COLUMNS; }
int SudokuGrid::getRows() { return ROWS; }
int SudokuGrid::getBlockPxSize() { return PIXELS_BLOCK_SIZE; }
const std::vector<std::vector<int>>& SudokuGrid::getDrawnGrid() const { return drawnGrid; }
int SudokuGrid::getSelectedNumber() const {
    return drawnGrid[selectedCell.y][selectedCell.x];
}

void SudokuGrid::resetGrid() {
    for (auto &row : grid) std::fill(row.begin(), row.end(), 0);
}

//--------------------CHECK UTILS--------------------
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

//--------------------SOLVING SUDOKU--------------------
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

//--------------------PRINTING & DRAWING--------------------
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
        sf::Color color = (i % 3 == 0) ? sf::Color::Black : GRID_INNER_LINE_COLOR;
        sf::RectangleShape vLine{sf::Vector2f(thickness, GRID_LINE_DIM)};
        vLine.setPosition(sf::Vector2f(GRID_LINE_X + float (PIXELS_BLOCK_SIZE * i), GRID_LINE_Y));
        vLine.setFillColor(color);
        WINDOW.draw(vLine);
    }
}
void SudokuGrid::drawHorizontalLines(sf::Color col) {
    for (int i = 0; i < ROWS + 1; i++) {
        float thickness = (i % 3 == 0) ? GRID_MAX_THICKNESS : GRID_MIN_THICKNESS;
        sf::Color color = (i % 3 == 0) ? sf::Color::Black : GRID_INNER_LINE_COLOR;
        sf::RectangleShape vLine{sf::Vector2f(GRID_LINE_DIM, thickness)};
        vLine.setPosition(sf::Vector2f(GRID_LINE_X, GRID_LINE_Y + float (PIXELS_BLOCK_SIZE * i)));
        vLine.setFillColor(color);
        WINDOW.draw(vLine);
    }
}
void SudokuGrid::drawNumbers(std::vector<std::vector<int>>& drawnGrid, const sf::Font& font, sf::Color col) {
    int txtSize = 30;
    sf::Text num(font);
    num.setFillColor(GRID_COLOR);
    num.setCharacterSize(txtSize);
    for (int row = 0; row < 9; row++) {
        for (int column = 0; column < 9; column++) {
            int actualNum = drawnGrid[column][row];
            num.setString(std::to_string(actualNum));

            sf::FloatRect bounds = num.getLocalBounds();

            int xOffset = 8;
            int yOffset = 10;
            float numX = GRID_LINE_X + (PIXELS_BLOCK_SIZE * row) + (bounds.size.x) + xOffset;
            float numY = GRID_LINE_Y + (PIXELS_BLOCK_SIZE * column) + (bounds.size.y) - yOffset;

            num.setPosition(sf::Vector2f(numX, numY));
            if (actualNum == 0) { continue; }
            WINDOW.draw(num);
        }
    }
}
void SudokuGrid::drawGrid(std::vector<std::vector<int>>& drawnGrid, sf::Color col) {
    highligtCell(selectedCell.x, selectedCell.y);
    highlightNumbers();
    drawHorizontalLines(col);
    drawVerticalLines(col);
    drawNumbers(drawnGrid, FONT, col);
}
void SudokuGrid::showSolution() { drawnGrid = grid; }

//--------------------MANAGE SELECTIONS--------------------
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
sf::Vector2i SudokuGrid::selectCell(int row, int col) {
    return  {row, col};
}

//--------------------MANAGE MOUSE--------------------
bool SudokuGrid::mouseXPosIsWithinGrid(sf::Vector2i mousePos) const {
    return (mousePos.x > GRID_LINE_X && mousePos.x < GRID_LINE_X + GRID_LINE_DIM);
}
bool SudokuGrid::mouseYPosIsWithinGrid(sf::Vector2i mousePos) const {
    return (mousePos.y > GRID_LINE_Y && mousePos.y < GRID_LINE_Y + GRID_LINE_DIM);
}
bool SudokuGrid::mousePosIsWithinGrid(sf::Vector2i mousePos) const {
    return mouseXPosIsWithinGrid(mousePos) && mouseYPosIsWithinGrid(mousePos);
}
void SudokuGrid::manageMousePosition() {
    rowIndexSelected = getSelectedRowIndex();
    colIndexSelected = getSelectedColumnIndex();
    if (Mouse::mouseLeftClicked() && mousePosIsWithinGrid(sf::Mouse::getPosition(WINDOW))) {
        selectedCell = selectCell(rowIndexSelected, colIndexSelected);
    }
}

//--------------------MANAGE CELLS TO HIDE--------------------
int SudokuGrid::manageCellsToHideN(DIFFICULTY difficulty) const {
    switch (difficulty) {
        case EASY: return 15;
        case MEDIUM: return 20;
        case HARD: return 25;
        case EXTREME: return 30;
        case SUICIDE: return 35;
        case IN_ASS_PAIN: return 50;
    }
    return -1;
}
void SudokuGrid::hideCell(int row, int col) {
    playerGrid[col][row] = 0;
}
void SudokuGrid::hideCells(DIFFICULTY difficulty) {
    std::vector<int> indices(81);
    std::iota(indices.begin(), indices.end(), 0);
    static std::mt19937 gen(std::random_device{}());
    std::shuffle(indices.begin(), indices.end(), gen);

    int cellsToHideN = manageCellsToHideN(difficulty);

    for (int i = 0; i < cellsToHideN; i++) {
        int idx = indices[i];
        int row = idx / 9;
        int col = idx % 9;
        hideCell(row, col);
    }
}

//--------------------MANAGE HIGHLIGHTS--------------------
void SudokuGrid::highlightRow(int selectedRow) {
    sf::RectangleShape rect(sf::Vector2f(PIXELS_BLOCK_SIZE, PIXELS_BLOCK_SIZE));
    sf::Color color = GRID_HIGHLIGHT_COLOR;
    rect.setFillColor(color);
    for (int col = 0; col < COLUMNS; col++) {
        rect.setPosition(sf::Vector2f(GRID_LINE_X + selectedRow * PIXELS_BLOCK_SIZE, GRID_LINE_Y + col * PIXELS_BLOCK_SIZE));
        WINDOW.draw(rect);
    }
}
void SudokuGrid::highlightColumn(int selectedCol) {
    sf::RectangleShape rect(sf::Vector2f(PIXELS_BLOCK_SIZE, PIXELS_BLOCK_SIZE));
    sf::Color color = GRID_HIGHLIGHT_COLOR;
    rect.setFillColor(color);
    for (int row = 0; row < ROWS; row++) {
        rect.setPosition(sf::Vector2f(GRID_LINE_X + row * PIXELS_BLOCK_SIZE, GRID_LINE_Y + selectedCol * PIXELS_BLOCK_SIZE));
        WINDOW.draw(rect);
    }
}
void SudokuGrid::highligtCell(int selectedRow, int selectedCol) {
    highlightRow(selectedRow);
    highlightColumn(selectedCol);
}
void SudokuGrid::highlightNumbers() {
    int selectedNum = getSelectedNumber();
    sf::RectangleShape rect(sf::Vector2f(PIXELS_BLOCK_SIZE, PIXELS_BLOCK_SIZE));
    rect.setFillColor(GRID_HIGHLIGHT_NUMBER_CELL_COLOR);

    for (int row = 0; row < ROWS; row++) {
        for (int col = 0; col < COLUMNS; col++) {
            int actualNum = drawnGrid[row][col];
            if ((actualNum == selectedNum) && (actualNum != 0)) {
                int rectX = GRID_LINE_X + col * PIXELS_BLOCK_SIZE;
                int rectY = GRID_LINE_Y + row * PIXELS_BLOCK_SIZE;
                rect.setPosition(sf::Vector2f(rectX, rectY));
                WINDOW.draw(rect);
            }
        }
    }
}

//--------------------MAIN GRID FUNCTION--------------------
void SudokuGrid::updateGrid(sf::Color col) {
    manageMousePosition();
    drawnGrid = playerGrid;
    if (Mouse::mouseRightPressed()) { showSolution(); }
    drawGrid(drawnGrid, col);
}