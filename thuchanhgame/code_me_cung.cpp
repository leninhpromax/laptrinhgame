#include <algorithm>
#include <random>
#include <ctime>
#include <iostream>
#include <vector>
#include <SDL.h>
#include <SDL_image.h>
#include "code_me_cung.h"

std::vector<std::vector<int>> maze(ROWS, std::vector<int>(COLUMNS, 0)); // Mảng lưu trữ mê cung

// Tạo mê cung ban đầu
void CreateMaze(std::vector<std::vector<int>>& maze) {
    for (int i = 0; i < ROWS; i++) {
        for (int j = 0; j < COLUMNS; j++) {
            maze[i][j] = 1; // Gán giá trị 1 cho tất cả các ô trong mê cung, biểu thị cho không gian trống
        }
    }
}

// Tạo tường ngẫu nhiên trong mê cung
void GenerateRandomWalls(std::vector<std::vector<int>>& maze) {
    // Khởi tạo thiết bị tạo số ngẫu nhiên
    std::random_device rd;

    // Khởi tạo bộ tạo số ngẫu nhiên Mersenne Twister
    std::mt19937 gen(rd());

    // Tạo phân phối số nguyên đồng đều trong phạm vi [0, ROWS * COLUMNS - 1]
    std::uniform_int_distribution<> dist(0, ROWS * COLUMNS - 1);

    // Tính số lượng ô tường cần tạo dựa trên tỷ lệ
    int numWalls = (WALL_RATIO * ROWS * COLUMNS) / 75;

    // Đảm bảo ít nhất một ô tường được tạo ra
    numWalls = std::max(numWalls, 1);

    // Số lượng ô tường đã được tạo
    int wallCount = 0;

    // Lặp lại để tạo các ô tường ngẫu nhiên
    while (wallCount < numWalls) {
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
void FindRewards(std::vector<std::vector<int>>& maze) {
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
                if (count == 2) {
                    maze[i][j] = 3;
                }
            }
        }
    }
}

// Sửa lỗi mê cung
void FixMazeError(std::vector<std::vector<int>>& maze) {
    int i = 1;
    int j = 1;
    for (int di = 1; di <= 3; di++) {
        for (int dj = 1; dj <= 3; dj++) {
            // Kiểm tra xem truy cập có vượt ra ngoài giới hạn không
            if (i + di < maze.size() && j + dj < maze[i].size()) {
                maze[i + di][j + dj] = 1;
            }
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
void BreakWalls(std::vector<std::vector<int>>& maze) {
    for (int i = 1; i < ROWS - 1; i++) {
        for (int j = 1; j < COLUMNS - 1; j++) {
            if (maze[i][j] == 0) {
                int count = 0;
                for (int di =-1; di <= 1; di ++){
                    for (int dj = -1; dj <= 1; dj ++){
                        if (i + di > 1 && i+di < ROWS-1 && j + dj > 1 && j + dj < COLUMNS-1){
                            if (maze[i+di][j+dj] == 0){
                                count ++;
                            }
                        }
                    }
                }
                if (count == 3){
                    maze[i][j] = 4;
                }
            }
        }
    }
}

// Hàm để di chuyển người chơi trong mê cung
void movePlayer(int& playerRow, int& playerCol, std::vector<std::vector<int>>& maze, SDL_Renderer* renderer, SDL_Texture* player) {
  SDL_Event e;
  while (SDL_PollEvent(&e) != 0) {
    if (e.type == SDL_KEYDOWN) {
      switch (e.key.keysym.sym) {
        case SDLK_w:
          if (playerRow > 0 && maze[playerRow - 1][playerCol] != 0) {
            maze[playerRow][playerCol] = 1;
            playerRow--;
            maze[playerRow][playerCol] = 5;
          }
          break;
        case SDLK_s:
          if (playerRow < ROWS - 1 && maze[playerRow + 1][playerCol] != 0) {
            maze[playerRow][playerCol] = 1;
            playerRow++;
            maze[playerRow][playerCol] = 5;
          }
          break;
        case SDLK_a:
          if (playerCol > 0 && maze[playerRow][playerCol - 1] != 0) {
            maze[playerRow][playerCol] = 1;
            playerCol--;
            maze[playerRow][playerCol] = 5;
          }
          break;
        case SDLK_d:
          if (playerCol < COLUMNS - 1 && maze[playerRow][playerCol + 1] != 0) {
            maze[playerRow][playerCol] = 1;
            playerCol++;
            maze[playerRow][playerCol] = 5;
          }
          break;
      }
    }
  }
  renderMaze(maze, player, nullptr, nullptr, renderer); // Vẽ lại mê cung sau khi di chuyển người chơi
  SDL_RenderPresent(renderer); // Hiển thị lên màn hình
  SDL_Delay(100); // Đợi một chút để tránh di chuyển quá nhanh
}

// Tìm một ô trống ở cuối mê cung để làm đích
std::pair<int, int> FindEmptySpace() {
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

// Khai báo hàm mới để vẽ mê cung
void renderMaze(std::vector<std::vector<int>>& maze,SDL_Texture* player, SDL_Texture* target,SDL_Texture* target2, SDL_Renderer* renderer) {

    for (int i = 0; i < ROWS; i++) {
        for (int j = 0; j < COLUMNS; j++) {
            if (maze[i][j] == 6) {
                renderTexture(target, 16 * j, 16 * i, 16, 16, renderer); // In ra vị trí của điểm đích
            } else if (maze[i][j] == 0) {
                SDL_Rect wallRect = {16 * j, 16 * i, 16, 16};
                SDL_SetRenderDrawColor(renderer, 255, 0, 0, 255); // Màu đỏ cho ô có tường
                SDL_RenderFillRect(renderer, &wallRect);
            } else if (maze[i][j] == 1) {
                SDL_Rect emptyRect = {16 * j, 16 * i, 16, 16};
                SDL_SetRenderDrawColor(renderer, 0, 255, 0, 255); // Màu xanh lá cho ô trống
                SDL_RenderFillRect(renderer, &emptyRect);
            } else if (maze[i][j] == 2) {
                renderTexture(target, 16 * j, 16 * i, 16, 16, renderer); // In ra điểm thưởng
            } else if (maze[i][j] == 3) {
                SDL_Rect hiddenRect = {16 * j, 16 * i, 16, 16};
                SDL_SetRenderDrawColor(renderer, 255, 255, 0, 255); // Màu vàng cho nơi ẩn
                SDL_RenderFillRect(renderer, &hiddenRect);
            } else if (maze[i][j] == 4) {
                SDL_Rect secretGateRect = {16 * j, 16 * i, 16, 16};
                SDL_SetRenderDrawColor(renderer, 255, 0, 255, 255); // Màu magenta cho cổng bí mật
                SDL_RenderFillRect(renderer, &secretGateRect);
            } else if (maze[i][j] == 5) {
                renderTexture(player, 16 * j, 16 * i, 16, 16, renderer); // In ra vị trí người chơi
            }
        }
    }
}
