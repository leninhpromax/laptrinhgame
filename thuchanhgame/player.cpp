#include "player.h"

Player::Player(Timer& timer, Maze& maze) : myTimer(timer), mazeG(maze) {
    playerRow = 1;
    playerCol = 1;
    blood = 500;
    score = 0;
    breakCount = 5;
    hiddenCount = 0;
    gameFinished = false;
    gameQuit = false;
}

void Player::StartGame() {
    mazeG.Game();
    mazek()[playerRow][playerCol] = 5;
    myTimer.start();
}


std::vector<std::vector<int>>& Player::mazek() {
    return mazeG.getMaze();
}

int& Player::Replace(int row, int col) {
    return mazek()[row][col];
}

void Player::Move() {
    SDL_Event e;
    if (!isGameQuit()){
        while (SDL_PollEvent(&e) != 0) {
            if (e.type == SDL_KEYDOWN) {
                switch (e.key.keysym.sym) {
                    case SDLK_q:
                        setGameQuit(true);
                        myTimer.pause();
                        break;
                    case SDLK_w:
                        if (playerRow > 0 && mazek()[playerRow - 1][playerCol] != 0 && mazek()[playerRow - 1][playerCol] != 6 && blood > 0) {
                            if (mazek()[playerRow - 1][playerCol] == 4) {
                                if (breakCount > 0) {
                                    breakCount--;
                                    mazek()[playerRow - 1][playerCol] = 1;
                                } else {
                                    break;
                                }
                            }
                            mazek()[playerRow][playerCol] = 1;
                            playerRow--;
                            if (mazek()[playerRow][playerCol] == 2) {
                                int randomScore = rand() % 3 + 1;
                                score += randomScore;
                            } else if (mazek()[playerRow][playerCol] == 3) {
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
                            } else if (mazek()[playerRow][playerCol] == 4) {
                                breakCount--;
                            }
                            mazek()[playerRow][playerCol] = 5;
                        }
                        else if (mazek()[playerRow][playerCol - 1] == 6 || blood <= 0){
                            setGameFinished(true);
                        }
                        break;
                    case SDLK_s:
                        if (playerRow < ROWS - 1 && mazek()[playerRow + 1][playerCol] != 0 && mazek()[playerRow + 1][playerCol] != 6) {
                            if (mazek()[playerRow + 1][playerCol] == 4) {
                                if (breakCount > 0) {
                                    breakCount--;
                                    mazek()[playerRow + 1][playerCol] = 1;
                                } else {
                                    break;
                                }
                            }
                            mazek()[playerRow][playerCol] = 1;
                            playerRow++;
                            if (mazek()[playerRow][playerCol] == 2) {
                                int randomScore = rand() % 3 + 1;
                                score += randomScore;
                            } else if (mazek()[playerRow][playerCol] == 3) {
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
                            } else if (mazek()[playerRow][playerCol] == 4) {
                                breakCount--;
                            }
                            mazek()[playerRow][playerCol] = 5;
                        }
                        else if (mazek()[playerRow + 1][playerCol] == 6 || blood <= 0){
                            setGameFinished(true);
                        }
                        break;
                    case SDLK_a:
                        if (playerCol > 0 && mazek()[playerRow][playerCol - 1] != 0 && mazek()[playerRow][playerCol - 1] != 6) {
                            if (mazek()[playerRow][playerCol - 1] == 4) {
                                if (breakCount > 0) {
                                    breakCount--;
                                    mazek()[playerRow][playerCol - 1] = 1;
                                } else {
                                    break;
                                }
                            }
                            mazek()[playerRow][playerCol] = 1;
                            playerCol--;
                            if (mazek()[playerRow][playerCol] == 2) {
                                int randomScore = rand() % 3 + 1;
                                score += randomScore;
                            } else if (mazek()[playerRow][playerCol] == 3) {
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
                            } else if (mazek()[playerRow][playerCol] == 4) {
                                breakCount--;
                            }
                            mazek()[playerRow][playerCol] = 5;
                        }
                        else if (mazek()[playerRow][playerCol - 1] == 6 || blood <= 0){
                            setGameFinished(true);
                        }
                        break;
                    case SDLK_d:
                        if (playerCol < COLUMNS - 1 && mazek()[playerRow][playerCol + 1] != 0 && mazek()[playerRow][playerCol + 1] != 6) {
                            if (mazek()[playerRow][playerCol + 1] == 4) {
                                if (breakCount > 0) {
                                    breakCount--;
                                    mazek()[playerRow][playerCol + 1] = 1;
                                } else {
                                    break;
                                }
                            }
                            mazek()[playerRow][playerCol] = 1;
                            playerCol++;
                            if (mazek()[playerRow][playerCol] == 2) {
                                int randomScore = rand() % 3 + 1;
                                score += randomScore;
                            } else if (mazek()[playerRow][playerCol] == 3) {
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
                            } else if (mazek()[playerRow][playerCol] == 4) {
                                breakCount--;
                            }
                            mazek()[playerRow][playerCol] = 5;
                        }
                        else if (mazek()[playerRow][playerCol + 1] == 6 || blood <= 0){
                            setGameFinished(true);
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
                        setGameFinished(true);
                        break;
                    case SDLK_q:
                        setGameQuit(false);
                        myTimer.unpause();
                        break;
                }
            }
        }
    }
    SDL_Delay(10); // Đợi một chút để tránh di chuyển quá nhanh
    RenderMaze(renderer, font, player, target, target2); // Vẽ lại mê cung sau khi di chuyển người chơi
}

void Player::RenderMaze(SDL_Renderer* renderer, TTF_Font* font, SDL_Texture* player, SDL_Texture* target, SDL_Texture* target2) {
    int widthl = 800; // Kích thước màn hình là 1200x800
    int heightl = 800;
    int MAZE_SIZE = 100; // Kích thước mê cung

     // Kiểm tra xem người chơi có nằm trong phạm vi màn hình hiển thị không
    if (playerRow <= 20 || playerCol <= 20 || playerRow >= MAZE_SIZE - 20 || playerCol >= MAZE_SIZE - 20) {
    // Tính toán vị trí camera để người chơi luôn ở giữa màn hình
    int cameraX = playerCol * CELL_SIZE - (widthl / 2 - CELL_SIZE / 2);
    int cameraY = playerRow * CELL_SIZE - (heightl / 2 - CELL_SIZE / 2);

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
                if (x >= -CELL_SIZE && x <= widthl && y >= -CELL_SIZE && y <= heightl) {
                    SDL_Rect rect = {x, y, CELL_SIZE, CELL_SIZE};
                    switch (mazek()[i][j]) {
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
int cameraX = playerX - widthl / 2;
int cameraY = playerY - heightl / 2;

// Clamp camera position to stay within maze boundaries
cameraX = std::max(0, std::min(cameraX, (MAZE_SIZE * CELL_SIZE) - widthl));
cameraY = std::max(0, std::min(cameraY, (MAZE_SIZE * CELL_SIZE) - heightl));

// Vẽ chỉ phần màn hình nhỏ xung quanh người chơi
for (int i = 0; i < heightl / CELL_SIZE; i++) {
    for (int j = 0; j < widthl / CELL_SIZE; j++) {
        // Calculate maze coordinates with camera offset
        int mazeX = (cameraX + j * CELL_SIZE) / CELL_SIZE;
        int mazeY = (cameraY + i * CELL_SIZE) / CELL_SIZE;

        // Calculate screen coordinates with camera offset
        int x = j * CELL_SIZE - (cameraX % CELL_SIZE);
        int y = i * CELL_SIZE - (cameraY % CELL_SIZE);

        // Kiểm tra xem ô có nằm trong mê cung không
        if (mazeX >= 0 && mazeX < MAZE_SIZE && mazeY >= 0 && mazeY < MAZE_SIZE) {
            SDL_Rect rect = {x, y, CELL_SIZE, CELL_SIZE};
            switch (mazek()[mazeY][mazeX]) {
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
}

void Player::WinnerScreen(SDL_Renderer* renderer, TTF_Font* font) {
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
