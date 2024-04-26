#include <chrono>
#include <algorithm>
#include <random>
#include <ctime>
#include <iostream>
#include <vector>
#include <SDL.h>
#include <SDL_image.h>
#include "code_me_cung.h"

Maze::Maze() : maze(ROWS, std::vector<int>(COLUMNS)) {}

std::vector<std::vector<int>>& Maze::getMaze() {
    return maze;
}
// Tạo mê cung ban đầu
void Maze::Create() {
    for (int i = 0; i < ROWS; i++) {
        for (int j = 0; j < COLUMNS; j++) {
            maze[i][j] = 1;
        }
    }
}

// Tạo tường ngẫu nhiên trong mê cung
void Maze::GenerateRandomWalls() {
    std::chrono::system_clock::time_point tp = std::chrono::system_clock::now();
    // Khởi tạo bộ tạo số ngẫu nhiên Mersenne Twister
    std::mt19937 gen(tp.time_since_epoch().count());
    // Tạo phân phối số nguyên đồng đều trong phạm vi [0, ROWS * COLUMNS - 1]
    std::uniform_int_distribution<> dist(0, ROWS * COLUMNS - 1);
    // Tính số lượng ô tường cần tạo dựa trên tỷ lệ
    int Walls = (ROWS * COLUMNS) / 12;
    // Số lượng ô tường đã được tạo
    int wallCount = 0;
    // Lặp lại để tạo các ô tường ngẫu nhiên
    while (wallCount <= Walls) {
        // Tạo chỉ số ngẫu nhiên trong phạm vi mảng
        int index = dist(gen);
        // Tính vị trí hàng và cột từ chỉ số
        int row = index / COLUMNS;
        int column = index % COLUMNS;
        // Kiểm tra ô hợp lệ (trống và nằm trong giới hạn mê cung)
        if (maze[row][column] == 1 && row > 0 && row < ROWS - 1 && column > 0 && column < COLUMNS - 1) {
            // Chọn hướng ngẫu nhiên (ngang = 0, dọc = 1)
            int direction = gen() % 2;
            // Độ dài ban đầu của ô tường
            int wall_length = 1;

            // Kiểm tra giới hạn trước khi vào vòng lặp để mở rộng ô tường
            if (direction == 0) { // Ngang
                if (column + 5 >= COLUMNS)
                    continue;
            } else { // Dọc
                if (row + 5 >= ROWS)
                    continue;
            }
            // Biến cờ xác định vị trí đặt hợp lệ
            bool valid_placement = true;
            // Lặp lại để mở rộng ô tường cho đến khi đạt độ dài tối đa hoặc gặp chướng ngại vật
            while (wall_length < 5 && valid_placement) {
                // Xác định hướng mở rộng
                if (direction == 0) { // Ngang
                    // Kiểm tra ô tiếp theo
                    if (maze[row][column + wall_length] != 1) {
                        valid_placement = false;
                    } else {
                        wall_length++;
                    }
                } else { // Dọc
                    // Kiểm tra ô tiếp theo
                    if (maze[row + wall_length][column] != 1) {
                        valid_placement = false;
                    } else {
                        wall_length++;
                    }
                }
            }
            // Tạo ô tường hợp lệ
            if (valid_placement) {
                // Lặp lại để đặt các ô theo chiều dài ô tường
                for (int j = 0; j < wall_length; j++) {
                    // Xác định vị trí đặt theo hướng
                    if (direction == 0) {
                        maze[row][column + j] = 0;
                    } else {
                        maze[row + j][column] = 0;
                    }
                }
                wallCount++; // Tăng số lượng ô tường đã tạo
            }
        }
    }
}


// Tìm vị trí các ô có điểm thưởng
void Maze::Rewards() {
    for (int i = 1; i < ROWS - 1; i++) {
        for (int j = 1; j < COLUMNS - 1; j++) {
            if (maze[i][j] == 1) {
                int wallCount = 0;
                // Kiểm tra các ô xung quanh
                if (maze[i][j - 1] == 0 && j-1 > 0 ) {
                    wallCount++;
                }
                if (maze[i][j + 1] == 0 && j+1 < COLUMNS-1) {
                    wallCount++;
                }
                if (maze[i + 1][j] == 0 && i+1 < ROWS-1) {
                    wallCount++;
                }
                if ( maze[i - 1][j] == 0 && i-1 > 0) {
                    wallCount++;
                }
                // Nếu có 3 ô trống xung quanh, đánh dấu ô hiện tại
                if (wallCount == 3) {
                    maze[i][j] = 2;
                }
            }
        }
    }

    // Kiểm tra ô xung quanh các ô có điểm thưởng để đánh dấu các ô nơi ẩn
    for (int i = 1; i < ROWS - 1; i++) {
        for (int j = 1; j < COLUMNS - 1; j++) {
            if (maze[i][j] == 1) {
                int count = 0;
                // Kiểm tra các ô xung quanh
                if (maze[i][j - 1] == 0 && maze[i-1][j] == 0 && j-1 > 0 && i-1 > 0) {
                    count++;
                }
                 if (maze[i][j + 1] == 0 && maze[i-1][j] == 0 && j+1 < COLUMNS-1 && i-1 > 0) {
                    count++;
                }
                 if (maze[i][j - 1] == 0 && maze[i+1][j] == 0 && j-1 > 0 && i+1 < ROWS-1) {
                    count++;
                }
                 if (maze[i][j + 1] == 0 && maze[i+1][j] == 0 && j+1 < COLUMNS-1 && i+1 < ROWS-1) {
                    count++;
                }
                // Nếu có 3 ô trống xung quanh, đánh dấu ô hiện tại
                if (count == 1) {
                    maze[i][j] = 3;
                }
            }
        }
    }
}

// Sửa lỗi mê cung
void Maze::FixError() {
    int ki = 1;
    int kj = 1;
    for (int di = 0; di <= 2; di++) {
        for (int dj = 0; dj <= 2; dj++) {
                maze[ki + di][kj + dj] = 1;
            }
    }
    // Đánh dấu các cạnh của mê cung thành tường
        for (int i = 0; i < ROWS; i++) {
            maze[i][0] = 0; // Cột đầu tiên
            maze[i][COLUMNS - 1] = 0; // Cột cuối cùng
        }
        for (int j = 0; j < COLUMNS; j++) {
            maze[0][j] = 0; // Hàng đầu tiên
            maze[ROWS - 1][j] = 0; // Hàng cuối cùng
        }
}

// Xử lý ô trống không kề tường
void Maze::BreakWalls() {
    for (int i = 1; i < ROWS - 1; i++) {
        for (int j = 1; j < COLUMNS - 1; j++) {
            if (maze[i][j] == 0) {
                int count = 0;
                if (maze[i][j + 1] == 0 || maze[i][j - 1] == 0) {
                    count = 1;
                }
                if (count == 0) {
                    maze[i][j] = 4;
                } else {
                    if (maze[i + 1][j] == 0 || maze[i - 1][j] == 0) {
                        count = 2;
                    }
                    if (count == 1) {
                        maze[i][j] = 4;
                    }
                }
            }
        }
    }
}

std::pair<int, int> Maze::EmptySpace() {
    int endRow = ROWS - 2; // Hàng cuối cùng
    int endCol = COLUMNS - 2; // Cột cuối cùng
    // Lặp qua các ô trong mê cung từ cuối mê cung lên trên và từ phải sang trái
    for (int i = endRow; i >= 0; i--) {
        for (int j = endCol; j >= 0; j--) {
            // Nếu ô hiện tại là trống và không phải là tường, trả về vị trí ô đó
            if (maze[i][j] == 1) {
                return std::make_pair(i, j);
            }
        }
    }
    // Nếu không tìm thấy ô trống, trả về vị trí (0, 0) (không hợp lệ)
    return std::make_pair(ROWS-2, COLUMNS-2);
}

int& Maze::Cells(int row, int col) {
    return maze[row][col];
}

void Maze::Game(){
    std::vector<std::vector<int>>& getMaze();
    Create();
    GenerateRandomWalls();
    Rewards();
    FixError();
    BreakWalls();
    std::pair<int, int> destination = EmptySpace();
    int endRow = destination.first;
    int endCol = destination.second;
    Cells(endRow, endCol) = 6;
}
