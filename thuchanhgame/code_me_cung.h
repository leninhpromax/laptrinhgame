#ifndef MAZE_H
#define MAZE_H

#include <chrono>
#include <algorithm>
#include <random>
#include <iostream>
#include <vector>
#include <utility>
#include <ctime>
#include <cstdlib>

const int ROWS = 100; // Số hàng của mê cung
const int COLUMNS = 100; // Số cột của mê cung

class Maze {
private:
    std::vector<std::vector<int>> maze;
public:
    Maze();
    std::vector<std::vector<int>>& getMaze();
    void Create();
    void GenerateRandomWalls();
    void Rewards();
    void FixError();
    void BreakWalls();
    std::pair<int, int> EmptySpace();
    std::pair<int, int> MiddleEmptySpace();
    int& Cells(int row, int col);
    void Game();
};

#endif
