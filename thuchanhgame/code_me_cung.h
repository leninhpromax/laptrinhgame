#ifndef MAZE_H
#define MAZE_H

#include <iostream>
#include <vector>
#include <algorithm>
#include <random>
#include <utility>
#include <ctime>
#include <cstdlib>
#include "man_hinh.h"

const int ROWS = 50; // Số hàng của mê cung
const int COLUMNS = 50; // Số cột của mê cung
const int WALL_RATIO = 5; // Tỷ lệ ô tường
const int GAME_DURATION = 720; // Thời gian giới hạn trò chơi (180 giây = 3 phút)

// Hàm này sẽ tính thời gian còn lại
int TimeLeft();


// Tạo mê cung ban đầu
void CreateMaze();

// Tạo tường ngẫu nhiên trong mê cung
void GenerateRandomWalls();

// Tìm vị trí các ô có điểm thưởng
void FindRewards();

// Sửa lỗi mê cung
void FixMazeError();

// Xử lý ô tường có thể phá được và ô tường không phá được
void BreakWalls();

// In ra mê cung với vị trí đích
void PrintMazeWithDestination(int endRow, int endCol);

// Di chuyển người chơi trong mê cung
void MovePlayer(int& playerRow, int& playerCol, int& score, int& breakCount, int& hiddenCount);

// Tìm một ô trống ở cuối mê cung để làm vị trí đích
std::pair<int, int> FindEmptySpace();

#endif
