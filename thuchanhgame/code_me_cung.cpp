#include <chrono>
#include <algorithm>
#include <random>
#include <ctime>
#include <iostream>
#include <vector>
#include <SDL.h>
#include <SDL_image.h>
#include "code_me_cung.h"
#include "time.h"

Maze::Maze() : maze(ROWS, std::vector<int>(COLUMNS, 0)) {}

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

std::vector<std::vector<int>>& Maze::getMaze() {
    return maze;
}

void movePlayer(int& playerRow, int& playerCol, int& blood, int& score, int& breakCount, int& hiddenCount, std::vector<std::vector<int>>& maze, SDL_Renderer* renderer, SDL_Texture* player, bool& kt, bool& pause, Timer& myTimer) {
  SDL_Event e;
  if (!pause){
  while (SDL_PollEvent(&e) != 0) {
    if (e.type == SDL_KEYDOWN) {
      switch (e.key.keysym.sym) {
     case SDL_QUIT:
          kt = true;
          break;
        case SDLK_q:
            pause = true;
            myTimer.pause();
          break;
        case SDLK_w:
          if (playerRow > 0 && maze[playerRow - 1][playerCol] != 0 && maze[playerRow - 1][playerCol] != 6 && blood > 0) {
                if (maze[playerRow - 1][playerCol] == 4) {
                    if (breakCount > 0) {
                        breakCount--;
                        maze[playerRow - 1][playerCol] = 1;
                    } else {
                        break;
                    }
                }
                maze[playerRow][playerCol] = 1;
                playerRow--;
                if (maze[playerRow][playerCol] == 2) {
                    int randomScore = rand() % 3 + 1;
                    score += randomScore;
                } else if (maze[playerRow][playerCol] == 3) {
                    hiddenCount++;
                    int hidden = rand()%4 + 1;
                    if (hidden == 1){
                        score ++;
                    }
                    else if (hidden == 2){
                        breakCount ++;
                    }
                    else if (hidden == 3){
                        int mau = (rand()%10 + 1)*10;
                        blood -= mau;
                    }
                    else {
                        if (blood <= 450 && blood > 0){
                        blood += 50;
                        }
                        else {
                            blood = 500;
                        }
                    }
                } else if (maze[playerRow][playerCol] == 4) {
                    breakCount--;
                }
                maze[playerRow][playerCol] = 5;
            }
            else if (maze[playerRow][playerCol - 1] == 6 || blood <= 0){
                kt = true;
            }
            break;
        case SDLK_s:
         if (playerRow < ROWS - 1 && maze[playerRow + 1][playerCol] != 0 && maze[playerRow + 1][playerCol] != 6) {
                if (maze[playerRow + 1][playerCol] == 4) {
                    if (breakCount > 0) {
                        breakCount--;
                        maze[playerRow + 1][playerCol] = 1;
                    } else {
                        break;
                    }
                }
                maze[playerRow][playerCol] = 1;
                playerRow++;
                if (maze[playerRow][playerCol] == 2) {
                    int randomScore = rand() % 3 + 1;
                    score += randomScore;
                } else if (maze[playerRow][playerCol] == 3) {
                     hiddenCount++;
                    int hidden = rand()%4 + 1;
                    if (hidden == 1){
                        score ++;
                    }
                    else if (hidden == 2){
                        breakCount ++;
                    }
                    else if (hidden == 3){
                        int mau = (rand()%10 + 1)*10;
                        blood -= mau;
                    }
                    else {
                        if (blood <= 450 && blood > 0){
                        blood += 50;
                        }
                        else {
                            blood = 500;
                        }
                    }
                } else if (maze[playerRow][playerCol] == 4) {
                    breakCount--;
                }
                maze[playerRow][playerCol] = 5;
            }
           else if (maze[playerRow + 1][playerCol] == 6 || blood <= 0){
                kt = true;
            }
            break;
        case SDLK_a:
           if (playerCol > 0 && maze[playerRow][playerCol - 1] != 0 && maze[playerRow][playerCol - 1] != 6) {
                if (maze[playerRow][playerCol - 1] == 4) {
                    if (breakCount > 0) {
                        breakCount--;
                        maze[playerRow][playerCol - 1] = 1;
                    } else {
                        break;
                    }
                }
                maze[playerRow][playerCol] = 1;
                playerCol--;
                if (maze[playerRow][playerCol] == 2) {
                    int randomScore = rand() % 3 + 1;
                    score += randomScore;
                } else if (maze[playerRow][playerCol] == 3) {
                    hiddenCount++;
                    int hidden = rand()%4 + 1;
                    if (hidden == 1){
                        score ++;
                    }
                    else if (hidden == 2){
                        breakCount ++;
                    }
                    else if (hidden == 3){
                        int mau = (rand()%10 + 1)*10;
                        blood -= mau;
                    }
                    else {
                        if (blood <= 450 && blood > 0){
                        blood += 50;
                        }
                        else {
                            blood = 500;
                        }
                    }
                } else if (maze[playerRow][playerCol] == 4) {
                    breakCount--;
                }
                maze[playerRow][playerCol] = 5;
            }
            else if (maze[playerRow][playerCol - 1] == 6 || blood <= 0){
                kt = true;
            }
          break;
        case SDLK_d:
         if (playerCol < COLUMNS - 1 && maze[playerRow][playerCol + 1] != 0 && maze[playerRow][playerCol + 1] != 6) {
                if (maze[playerRow][playerCol + 1] == 4) {
                    if (breakCount > 0) {
                        breakCount--;
                        maze[playerRow][playerCol + 1] = 1;
                    } else {
                        break;
                    }
                }
                maze[playerRow][playerCol] = 1;
                playerCol++;
                if (maze[playerRow][playerCol] == 2) {
                    int randomScore = rand() % 3 + 1;
                    score += randomScore;
                } else if (maze[playerRow][playerCol] == 3) {
                    hiddenCount++;
                    int hidden = rand()%4 + 1;
                    if (hidden == 1){
                        score ++;
                    }
                    else if (hidden == 2){
                        breakCount ++;
                    }
                    else if (hidden == 3){
                        int mau = (rand()%10 + 1)*10;
                        blood -= mau;
                    }
                    else {
                        if (blood <= 450 && blood > 0){
                        blood += 50;
                        }
                        else {
                            blood = 500;
                        }
                    }
                } else if (maze[playerRow][playerCol] == 4) {
                    breakCount--;
                }
                maze[playerRow][playerCol] = 5;
            }
            else if (maze[playerRow][playerCol + 1] == 6 || blood <= 0){
                kt = true;
            }
            break;
      }
    }
  }
  }
  else{
    while (SDL_PollEvent(&e) != 0) {
    if (e.type == SDL_KEYDOWN) {
      switch (e.key.keysym.sym) {
     case SDL_QUIT:
          kt = true;
          break;
        case SDLK_q:
            pause = false;
            myTimer.unpause();
          break;
  }
    }
    }
  }

  renderMaze(maze, player, nullptr, nullptr, renderer, playerRow, playerCol); // Vẽ lại mê cung sau khi di chuyển người chơi
  // Hiển thị thời gian
    std::stringstream time;
    time << "Timer: " << myTimer.get_ticks() / 1000.f;
    renderText(renderer, font, time.str().c_str(), 850, 100);

    std::stringstream bloodString;
     bloodString << "Blood: " << blood;
    // Vẽ bloodString lên renderer
    renderText(renderer, font, bloodString.str().c_str(), 850, 150);

    std::stringstream scoreString;
     scoreString << "Score: " << score;
    // Vẽ scoreString lên renderer
    renderText(renderer, font, scoreString.str().c_str(), 850, 200);

     std::stringstream hiddenCountString;
     hiddenCountString << "hiddenCount: " << hiddenCount;
    // Vẽ scoreString lên renderer
    renderText(renderer, font, hiddenCountString.str().c_str(), 850, 250);

     std::stringstream breakCountString;
     breakCountString << "breakCount: " << breakCount;
    // Vẽ scoreString lên renderer
    renderText(renderer, font, breakCountString.str().c_str(), 850, 300);

  SDL_RenderPresent(renderer); // Hiển thị lên màn hình
  SDL_Delay(10); // Đợi một chút để tránh di chuyển quá nhanh
}

void renderMaze(std::vector<std::vector<int>>& maze, SDL_Texture* player, SDL_Texture* target, SDL_Texture* target2, SDL_Renderer* renderer, int& playerRow, int& playerCol) {
    const int VIEW_SIZE = 40; // Kích thước màn hình 35x35 ô
    const int SCREEN_WIDTH = 800; // Kích thước màn hình là 1200x800
    const int SCREEN_HEIGHT = 800;
    const int CELL_SIZE = 20; // Kích thước của mỗi ô
    const int MAZE_SIZE = 100; // Kích thước mê cung

     // Kiểm tra xem người chơi có nằm trong phạm vi màn hình hiển thị không
    if (playerRow <= 20 || playerCol <= 20 || playerRow >= MAZE_SIZE - 20 || playerCol >= MAZE_SIZE - 20) {
    // Tính toán vị trí camera để người chơi luôn ở giữa màn hình
    int cameraX = playerCol * CELL_SIZE - (SCREEN_WIDTH / 2 - CELL_SIZE / 2);
    int cameraY = playerRow * CELL_SIZE - (SCREEN_HEIGHT / 2 - CELL_SIZE / 2);

    // Kiểm tra nếu camera vượt ra ngoài màn hình
    if (cameraX < 0) cameraX = 0;
    if (cameraY < 0) cameraY = 0;
    int maxCameraX = (MAZE_SIZE - VIEW_SIZE) * CELL_SIZE;
    int maxCameraY = (MAZE_SIZE - VIEW_SIZE) * CELL_SIZE;
    if (cameraX > maxCameraX) cameraX = maxCameraX;
    if (cameraY > maxCameraY) cameraY = maxCameraY;

        // Vẽ toàn bộ mê cung và người chơi ở vị trí hiện tại
        for (int i = 0; i < MAZE_SIZE; i++) {
            for (int j = 0; j < MAZE_SIZE; j++) {
                int x = j * CELL_SIZE - cameraX;
                int y = i * CELL_SIZE - cameraY;
                // Kiểm tra xem ô có nằm trong màn hình không
                if (x >= -CELL_SIZE && x <= SCREEN_WIDTH && y >= -CELL_SIZE && y <= SCREEN_HEIGHT) {
                    SDL_Rect rect = {x, y, CELL_SIZE, CELL_SIZE};
                    switch (maze[i][j]) {
                        case 0: // Tường
                            SDL_SetRenderDrawColor(renderer, 255, 0, 0, 255);
                            SDL_RenderFillRect(renderer, &rect);
                            break;
                        case 1: // Ô trống
                            SDL_SetRenderDrawColor(renderer, 0, 255, 0, 255);
                            SDL_RenderFillRect(renderer, &rect);
                            break;
                        case 2: // Điểm thưởng
                            renderTexture(target, x, y, CELL_SIZE, CELL_SIZE, renderer);
                            break;
                        case 3: // Nơi ẩn
                            SDL_SetRenderDrawColor(renderer, 255, 255, 0, 255);
                            SDL_RenderFillRect(renderer, &rect);
                            break;
                        case 4: // Cổng bí mật
                            SDL_SetRenderDrawColor(renderer, 255, 0, 255, 255);
                            SDL_RenderFillRect(renderer, &rect);
                            break;
                        case 5: // Vị trí người chơi
                            renderTexture(player, x, y, CELL_SIZE, CELL_SIZE, renderer);
                            break;
                        default:
                            break;
                    }
                }
            }
        }
    } else {

// Tính toán vị trí của người chơi trong mê cung
int playerX = playerCol * CELL_SIZE;
int playerY = playerRow * CELL_SIZE;

// Tính toán vị trí của camera để người chơi luôn ở trung tâm màn hình
int cameraX = playerX - SCREEN_WIDTH / 2;
int cameraY = playerY - SCREEN_HEIGHT / 2;

// Clamp camera position to stay within maze boundaries
cameraX = std::max(0, std::min(cameraX, (MAZE_SIZE * CELL_SIZE) - SCREEN_WIDTH));
cameraY = std::max(0, std::min(cameraY, (MAZE_SIZE * CELL_SIZE) - SCREEN_HEIGHT));

// Vẽ chỉ phần màn hình nhỏ xung quanh người chơi
for (int i = 0; i < SCREEN_HEIGHT / CELL_SIZE; i++) {
    for (int j = 0; j < SCREEN_WIDTH / CELL_SIZE; j++) {
        // Calculate maze coordinates with camera offset
        int mazeX = (cameraX + j * CELL_SIZE) / CELL_SIZE;
        int mazeY = (cameraY + i * CELL_SIZE) / CELL_SIZE;

        // Calculate screen coordinates with camera offset
        int x = j * CELL_SIZE - (cameraX % CELL_SIZE);
        int y = i * CELL_SIZE - (cameraY % CELL_SIZE);

        // Kiểm tra xem ô có nằm trong mê cung không
        if (mazeX >= 0 && mazeX < MAZE_SIZE && mazeY >= 0 && mazeY < MAZE_SIZE) {
            SDL_Rect rect = {x, y, CELL_SIZE, CELL_SIZE};
            switch (maze[mazeY][mazeX]) {
                case 0: // Tường
                    SDL_SetRenderDrawColor(renderer, 255, 0, 0, 255);
                    SDL_RenderFillRect(renderer, &rect);
                    break;
                case 1: // Ô trống
                    SDL_SetRenderDrawColor(renderer, 0, 255, 0, 255);
                    SDL_RenderFillRect(renderer, &rect);
                    break;
                case 2: // Điểm thưởng
                    renderTexture(target, x, y, CELL_SIZE, CELL_SIZE, renderer);
                    break;
                case 3: // Nơi ẩn
                    SDL_SetRenderDrawColor(renderer, 255, 255, 0, 255);
                    SDL_RenderFillRect(renderer, &rect);
                    break;
                case 4: // Cổng bí mật
                    SDL_SetRenderDrawColor(renderer, 255, 0, 255, 255);
                    SDL_RenderFillRect(renderer, &rect);
                    break;
                case 5: // Vị trí người chơi
                    renderTexture(player, x, y, CELL_SIZE, CELL_SIZE, renderer);
                    break;
                default:
                    break;
            }
        }
    }
}
    }
}


void clearScreen(SDL_Renderer* renderer, int score, TTF_Font* font, Timer& myTimer, int SCREEN_WIDTH, int SCREEN_HEIGHT) {
    // Xóa màn hình
    SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
    SDL_RenderClear(renderer);

    // Tính thời gian đã chơi
    float TimeInSeconds = myTimer.get_ticks() / 1000.f;

    std::stringstream winText;
    winText << "Win" ;
    renderText(renderer, font, winText.str().c_str(), (SCREEN_WIDTH - 400) / 2, (SCREEN_HEIGHT - 400) / 2);

    // Hiển thị điểm số và thời gian chơi
    std::stringstream scoreText;
    scoreText << "Your score: " << score << " seconds";
    renderText(renderer, font, scoreText.str().c_str(), (SCREEN_WIDTH - 400) / 2, (SCREEN_HEIGHT - 200) / 2);

    std::stringstream timeText;
    timeText << "Time: " << TimeInSeconds << " seconds";
    renderText(renderer, font, timeText.str().c_str(), (SCREEN_WIDTH - 400) / 2, (SCREEN_HEIGHT - 300) / 2);

    // Hiển thị lên màn hình
    SDL_RenderPresent(renderer);
}
