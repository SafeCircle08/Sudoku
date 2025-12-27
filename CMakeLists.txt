set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)
cmake_minimum_required(VERSION 3.28)
project(Sudoku LANGUAGES CXX)

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR})

include(FetchContent)
FetchContent_Declare(SFML
        GIT_REPOSITORY https://github.com/SFML/SFML.git
        GIT_TAG 3.0.2
        GIT_SHALLOW ON
        EXCLUDE_FROM_ALL
        SYSTEM)

FetchContent_MakeAvailable(SFML)

add_executable(Sudoku
        src/main.cpp
        header/SudokuGrid.h
        src/SudokuGrid.cpp
        header/Random.h
        src/Random.cpp
        src/Mouse.cpp
        header/Mouse.h
)

target_compile_features(Sudoku PRIVATE cxx_std_17)
target_link_libraries(Sudoku PRIVATE SFML::Graphics)