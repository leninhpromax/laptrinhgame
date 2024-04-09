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
#include "man_hinh.h"
#include "time.h"

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
    int& Cells(int row, int col);
};
// Hàm di chuyển người chơi
void movePlayer(int& playerRow, int& playerCol, int& blood, int& score, int& breakCount, int& hiddenCount, std::vector<std::vector<int>>& maze, SDL_Renderer* renderer, SDL_Texture* player, bool& kt, bool& pause, Timer& myTimer);

// Hàm tìm một ô trống ở cuối mê cung làm đích
std::pair<int, int> FindEmptySpace();

// Hàm vẽ mê cung
void renderMaze(std::vector<std::vector<int>>& maze, SDL_Texture* player, SDL_Texture* target, SDL_Texture* target2, SDL_Renderer* renderer, int& playerRow, int& playerCol);

void clearScreen(SDL_Renderer* renderer, int score, TTF_Font* font, Timer& myTimer, int SCREEN_WIDTH, int SCREEN_HEIGHT);

extern SDL_Texture* player;
extern SDL_Texture* target;
extern SDL_Texture* target2;
extern SDL_Renderer* renderer;

#endif
