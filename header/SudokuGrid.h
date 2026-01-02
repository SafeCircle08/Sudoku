#pragma once
#include <vector>
#include <SFML/Graphics.hpp>

struct INSERTION_CELL {
    int num;
    int timesInGrid;
    bool finished;
};

enum FILL_WAYS {
    FILL_IN_COLUMN,
    FILL_IN_ROW,
};

enum DIFFICULTY {
    EASY,
    MEDIUM,
    HARD,
    EXTREME,
    SUICIDE,
    IN_ASS_PAIN
};

inline std::vector<INSERTION_CELL> insertionCells;

#define COLOR_WHITE sf::Color(255,255,255)
#define COLOR_RED sf::Color(255,0,0)
#define COLOR_LIGHT_BLUE sf::Color(173, 216, 230, 255)
#define COLOR_MEDIUM_BLUE sf::Color(135, 206, 235, 255)
#define COLOR_SOFT_LIGHT_BLUE sf::Color(212, 239, 255, 255)
#define COLOR_PALLID_BLUE sf::Color(176, 224, 230, 255)
#define COLOR_CRAZY_LIGHT_BLUE sf::Color(0, 191, 255, 255)
#define COLOR_INTENSE_LIGHT_BLUE sf::Color(0, 128, 255, 255)
#define COLOR_SOFT_LIGHT_PURPLE sf::Color(184, 204, 255, 255)

class SudokuGrid {
private:
    sf::Color GRID_COLOR;
    sf::Color GRID_HIGHLIGHT_COLOR;
    sf::Color GRID_HIGHLIGHT_NUMBER_CELL_COLOR;
    sf::Color GRID_INNER_LINE_COLOR;

    sf::RenderWindow& WINDOW;
    const sf::Font FONT = sf::Font("C:/Users/Utente/AppData/Local/Microsoft/Windows/Fonts/big-shot.ttf");
    static constexpr int ROWS = 9;
    static constexpr int COLUMNS = 9;
    static constexpr int BLOCK_ROWS = 3;
    static constexpr int BLOCK_COLS = 3;
    static constexpr int PIXELS_BLOCK_SIZE = 60;
    static constexpr int GRID_MAX_THICKNESS = 3;
    static constexpr int GRID_MIN_THICKNESS = 1;
    const float GRID_LINE_DIM = (float) (PIXELS_BLOCK_SIZE * ROWS);
    float GRID_LINE_X;
    float GRID_LINE_Y;
    DIFFICULTY difficulty;

    std::vector<std::vector<int>> grid;
    std::vector<std::vector<int>> playerGrid;
    std::vector<std::vector<int>> drawnGrid = grid;

    int rowIndexSelected = -1;
    int colIndexSelected = -1;
    sf::Vector2i selectedCell;

    [[nodiscard]] bool checkRow(int row, int num) const;
    [[nodiscard]] bool checkColumn(int col, int num) const;
    [[nodiscard]] bool checkBlock(int row, int col, int num) const;
    [[nodiscard]] bool isValid(int row, int col, int num) const;
    void fillRow(int rowN);
    void fillColumn(int colN);

    void resetGrid();
    bool solveSudoku();
    void showSolution();
    bool insertionCellIsFinished(INSERTION_CELL& cell);
    void showInsertionCellInfo(INSERTION_CELL& cell) const;
    int getTimeNumberIsInGrid(int numToCheck);
    void updateInsertionCell();
    void loadInsertionCells();
    void manageFill();

    void printGrid() const;
    void drawVerticalLines(sf::Color col);
    void drawHorizontalLines(sf::Color col);
    void drawGrid(std::vector<std::vector<int>>& drawGrid, sf::Color col);
    void drawNumbers(std::vector<std::vector<int>>& drawnGrid, const sf::Font& font, sf::Color col);
    void drawInsertionCells();
    void insertNumber(int num);
    bool numberInsertedIsCorrect(int num);

    int getSelectedRowIndex() const;
    int getSelectedColumnIndex() const;
    int getSelectedNumber() const;

    void manageMousePosition();
    bool mouseXPosIsWithinGrid(sf::Vector2i mousePos) const;
    bool mouseYPosIsWithinGrid(sf::Vector2i mousePos) const;
    bool mousePosIsWithinGrid(sf::Vector2i mousePos) const;
    bool mouseIsWithinXCoordinates(int mouseMinXPos, int mouseMaxXPos) const;
    bool mouseIsWithinYCoordinates(int mouseMinYPos, int mouseMaxYPos) const;
    bool mousePosIsWithinCoordinates(sf::Vector2i minMousePos, sf::Vector2i maxMousePos) const;

    void hideCell(int row, int col);
    void hideCells(DIFFICULTY difficulty);
    int manageCellsToHideN(DIFFICULTY difficulty) const;

    sf::Vector2i selectCell(int row, int col);
    void highligtCell(int col, int row);
    void highlightRow(int row);
    void highlightColumn(int col);
    void highlightNumbers();
public:
    explicit SudokuGrid(
        DIFFICULTY difficulty, sf::RenderWindow& window, sf::Color gridColor,
        sf::Color hlColor, sf::Color hlNumColor, sf::Color innerLinesColor
    );

    const std::vector<std::vector<int>>& getDrawnGrid() const;
    static int getRows();
    static int getColumns();
    static int getBlockPxSize();
    void updateGrid(sf::Color col);
};