#ifndef MAZE_H
#define MAZE_H

#include <bits/stdc++.h>
#include <iostream>
#include <vector>
#include <algorithm>
#include <random>
#include <utility>
#include <ctime>
#include <cstdlib>
#include "man_hinh.h"
#include "time.h"

const int ROWS = 50; // Số hàng của mê cung
const int COLUMNS = 50; // Số cột của mê cung
const int WALL_RATIO = 5; // Tỷ lệ ô tường

// Hàm tạo mê cung
void CreateMaze(std::vector<std::vector<int>>& maze);

// Hàm tạo tường ngẫu nhiên
void GenerateRandomWalls(std::vector<std::vector<int>>& maze);

// Hàm tìm vị trí các ô có điểm thưởng
void FindRewards(std::vector<std::vector<int>>& maze);

// Hàm sửa lỗi mê cung
void FixMazeError(std::vector<std::vector<int>>& maze);

// Hàm xử lý ô trống không kề tường
void BreakWalls(std::vector<std::vector<int>>& maze);

// Hàm di chuyển người chơi
void movePlayer(int& playerRow, int& playerCol, int& blood, int& score, int& breakCount, int& hiddenCount, std::vector<std::vector<int>>& maze, SDL_Renderer* renderer, SDL_Texture* player, bool& kt);

// Hàm tìm một ô trống ở cuối mê cung làm đích
std::pair<int, int> FindEmptySpace();

// Hàm vẽ mê cung
void renderMaze(std::vector<std::vector<int>>& maze, SDL_Texture* player, SDL_Texture* target, SDL_Texture* target2, SDL_Renderer* renderer);

void clearScreenAndShowScore(SDL_Renderer* renderer, TTF_Font* font, Timer& myTimer, int SCREEN_WIDTH, int SCREEN_HEIGHT);

extern SDL_Texture* player;
extern SDL_Texture* target;
extern SDL_Texture* target2;
extern SDL_Renderer* renderer;

#endif
