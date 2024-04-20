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
    movedUp = false;
    movedDown = false;
    movedLeft = false;
    movedRight = false;
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
    if (!isGameQuit()) {
        while (SDL_PollEvent(&e) != 0) {
            if (e.type == SDL_KEYDOWN) {
                switch (e.key.keysym.sym) {
                    case SDLK_q:
                        setGameQuit(true);
                        myTimer.pause();
                        break;
                    case SDLK_w:
                        Moveup();
                        movedUp = true;
                        movedDown = false;
                        movedLeft = false;
                        movedRight = false;
                        break;
                    case SDLK_s:
                        Movedown();
                        movedUp = false;
                        movedDown = true;
                        movedLeft = false;
                        movedRight = false;
                        break;
                    case SDLK_a:
                        Moveleft();
                        movedUp = false;
                        movedDown = false;
                        movedLeft = true;
                        movedRight = false;
                        break;
                    case SDLK_d:
                       Moveright();
                        movedUp = false;
                        movedDown = false;
                        movedLeft = false;
                        movedRight = true;
                        break;
                    case SDLK_SPACE:
                        // Bắn viên đạn với hướng di chuyển hiện tại của người chơi
                         if (!bullet.isActive()) {
                            bullet.Shoot(getRow(), getCol(), hasMovedUp(), hasMovedDown(), hasMovedLeft(), hasMovedRight());
                        }
                        break;
                }
            } else if (e.type == SDL_QUIT) {
                setGameFinished(true);
            }
        }
    } else {
        while (SDL_PollEvent(&e) != 0) {
            if (e.type == SDL_KEYDOWN) {
                switch (e.key.keysym.sym) {
                    case SDLK_q:
                        setGameQuit(false);
                        myTimer.unpause();
                        break;
                    case SDLK_r:
                        restartGame();
                        break;
                }
            } else if (e.type == SDL_QUIT) {
                setGameFinished(true);
            }
        }
    }

    // Di chuyển viên đạn
    bullet.Move(mazek());

    SDL_Delay(10); // Đợi một chút để tránh di chuyển quá nhanh

    // Vẽ lại mê cung sau khi di chuyển người chơi
    RenderMaze();
}

void Player::RenderMaze() {
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
    time << "Timer: " << static_cast<int>(myTimer.get_ticks() / 1000.f);
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

void Player::WinnerScreen() {
    // Xóa màn hình
    SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
    SDL_RenderClear(renderer);

    // Tính thời gian đã chơi
    int TimeInSeconds = myTimer.get_ticks() / 1000.f;

    std::stringstream winText;
    winText << "Win" ;
    renderText(renderer, font, winText.str().c_str(), 400, 400);

    // Hiển thị điểm số và thời gian chơi
    std::stringstream scoreText;
    scoreText << "Your score: " << score << " seconds";
    renderText(renderer, font, scoreText.str().c_str(), 400, 600);

    std::stringstream timeText;
    timeText << "Time: " << TimeInSeconds << " seconds";
    renderText(renderer, font, timeText.str().c_str(), 400, 500);

    // Hiển thị lên màn hình
    SDL_RenderPresent(renderer);
}

void Player::Moveup() {
    if (playerRow > 0) {
        int nextCell = mazek()[playerRow - 1][playerCol]; // Ô tiếp theo mà người chơi sẽ di chuyển đến
        if (nextCell != 0 && nextCell != 6 && blood > 0) { // Kiểm tra ô tiếp theo không phải là tường hoặc ô đích và người chơi còn máu
            if (nextCell == 4) { // Nếu ô tiếp theo là gạch và người chơi còn cơ hội phá
                if (breakCount > 0) {
                    breakCount--;
                    mazek()[playerRow - 1][playerCol] = 1; // Phá gạch và di chuyển lên
                } else {
                    return; // Không thực hiện di chuyển nếu không còn cơ hội phá gạch
                }
            }
            mazek()[playerRow][playerCol] = 1; // Đánh dấu ô hiện tại là đã đi qua
            playerRow--; // Di chuyển lên
            if (nextCell == 2) { // Nếu ô tiếp theo là điểm
                int randomScore = rand() % 3 + 1;
                score += randomScore; // Tăng điểm
            } else if (nextCell == 3) { // Nếu ô tiếp theo là ô ẩn
                hiddenCount++;
                int hidden = rand() % 4 + 1;
                if (hidden == 1) {
                    score++;
                } else if (hidden == 2) {
                    breakCount++;
                } else if (hidden == 3) {
                    int mau = (rand() % 10 + 1) * 10;
                    blood -= mau;
                } else {
                    if (blood <= 450 && blood > 0) {
                        blood += 50;
                    } else {
                        blood = 500;
                    }
                }
            }
            mazek()[playerRow][playerCol] = 5; // Đánh dấu ô mới mà người chơi đến
        } else if (nextCell == 0) { // Nếu ô tiếp theo là tường
            return; // Không thực hiện di chuyển
        } else { // Nếu người chơi chạm vào tường hoặc hết máu
            setGameFinished(true);
        }
    } else { // Nếu người chơi ở hàng trên cùng của mê cung
        setGameFinished(true); // Kết thúc trò chơi
    }
}

void Player::Movedown() {
    if (playerRow < ROWS - 1) {
        int nextCell = mazek()[playerRow + 1][playerCol]; // Ô tiếp theo mà người chơi sẽ di chuyển đến
        if (nextCell != 0 && nextCell != 6 && blood > 0) { // Kiểm tra ô tiếp theo không phải là tường hoặc ô đích và người chơi còn máu
            if (nextCell == 4) { // Nếu ô tiếp theo là gạch và người chơi còn cơ hội phá
                if (breakCount > 0) {
                    breakCount--;
                    mazek()[playerRow + 1][playerCol] = 1; // Phá gạch và di chuyển xuống
                } else {
                    return; // Không thực hiện di chuyển nếu không còn cơ hội phá gạch
                }
            }
            mazek()[playerRow][playerCol] = 1; // Đánh dấu ô hiện tại là đã đi qua
            playerRow++; // Di chuyển xuống
            if (nextCell == 2) { // Nếu ô tiếp theo là điểm
                int randomScore = rand() % 3 + 1;
                score += randomScore; // Tăng điểm
            } else if (nextCell == 3) { // Nếu ô tiếp theo là ô ẩn
                hiddenCount++;
                int hidden = rand() % 4 + 1;
                if (hidden == 1) {
                    score++;
                } else if (hidden == 2) {
                    breakCount++;
                } else if (hidden == 3) {
                    int mau = (rand() % 10 + 1) * 10;
                    blood -= mau;
                } else {
                    if (blood <= 450 && blood > 0) {
                        blood += 50;
                    } else {
                        blood = 500;
                    }
                }
            }
            mazek()[playerRow][playerCol] = 5; // Đánh dấu ô mới mà người chơi đến
        } else if (nextCell == 0) { // Nếu ô tiếp theo là tường
            return; // Không thực hiện di chuyển
        } else { // Nếu người chơi chạm vào tường hoặc hết máu
            setGameFinished(true);
        }
    } else { // Nếu người chơi ở hàng dưới cùng của mê cung
        setGameFinished(true); // Kết thúc trò chơi
    }
}

void Player::Moveleft() {
    if (playerCol > 0) {
        int nextCell = mazek()[playerRow][playerCol - 1]; // Ô tiếp theo mà người chơi sẽ di chuyển đến
        if (nextCell != 0 && nextCell != 6 && blood > 0) { // Kiểm tra ô tiếp theo không phải là tường hoặc ô đích và người chơi còn máu
            if (nextCell == 4) { // Nếu ô tiếp theo là gạch và người chơi còn cơ hội phá
                if (breakCount > 0) {
                    breakCount--;
                    mazek()[playerRow][playerCol - 1] = 1; // Phá gạch và di chuyển sang trái
                } else {
                    return; // Không thực hiện di chuyển nếu không còn cơ hội phá gạch
                }
            }
            mazek()[playerRow][playerCol] = 1; // Đánh dấu ô hiện tại là đã đi qua
            playerCol--; // Di chuyển sang trái
            if (nextCell == 2) { // Nếu ô tiếp theo là điểm
                int randomScore = rand() % 3 + 1;
                score += randomScore; // Tăng điểm
            } else if (nextCell == 3) { // Nếu ô tiếp theo là ô ẩn
                hiddenCount++;
                int hidden = rand() % 4 + 1;
                if (hidden == 1) {
                    score++;
                } else if (hidden == 2) {
                    breakCount++;
                } else if (hidden == 3) {
                    int mau = (rand() % 10 + 1) * 10;
                    blood -= mau;
                } else {
                    if (blood <= 450 && blood > 0) {
                        blood += 50;
                    } else {
                        blood = 500;
                    }
                }
            }
            mazek()[playerRow][playerCol] = 5; // Đánh dấu ô mới mà người chơi đến
        } else if (nextCell == 0) { // Nếu ô tiếp theo là tường
            return; // Không thực hiện di chuyển
        } else { // Nếu người chơi chạm vào tường hoặc hết máu
            setGameFinished(true);
        }
    } else { // Nếu người chơi ở cột bên trái của mê cung
        setGameFinished(true); // Kết thúc trò chơi
    }
}

void Player::Moveright() {
    if (playerCol < COLUMNS - 1) {
        int nextCell = mazek()[playerRow][playerCol + 1]; // Ô tiếp theo mà người chơi sẽ di chuyển đến
        if (nextCell != 0 && nextCell != 6 && blood > 0) { // Kiểm tra ô tiếp theo không phải là tường hoặc ô đích và người chơi còn máu
            if (nextCell == 4) { // Nếu ô tiếp theo là gạch và người chơi còn cơ hội phá
                if (breakCount > 0) {
                    breakCount--;
                    mazek()[playerRow][playerCol + 1] = 1; // Phá gạch và di chuyển sang phải
                } else {
                    return; // Không thực hiện di chuyển nếu không còn cơ hội phá gạch
                }
            }
            mazek()[playerRow][playerCol] = 1; // Đánh dấu ô hiện tại là đã đi qua
            playerCol++; // Di chuyển sang phải
            if (nextCell == 2) { // Nếu ô tiếp theo là điểm
                int randomScore = rand() % 3 + 1;
                score += randomScore; // Tăng điểm
            } else if (nextCell == 3) { // Nếu ô tiếp theo là ô ẩn
                hiddenCount++;
                int hidden = rand() % 4 + 1;
                if (hidden == 1) {
                    score++;
                } else if (hidden == 2) {
                    breakCount++;
                } else if (hidden == 3) {
                    int mau = (rand() % 10 + 1) * 10;
                    blood -= mau;
                } else {
                    if (blood <= 450 && blood > 0) {
                        blood += 50;
                    } else {
                        blood = 500;
                    }
                }
            }
            mazek()[playerRow][playerCol] = 5; // Đánh dấu ô mới mà người chơi đến
        } else if (nextCell == 0) { // Nếu ô tiếp theo là tường
            return; // Không thực hiện di chuyển
        } else { // Nếu người chơi chạm vào tường hoặc hết máu
            setGameFinished(true);
        }
    } else { // Nếu người chơi ở cột bên trái của mê cung
        setGameFinished(true); // Kết thúc trò chơi
    }
}

void Player::restartGame() {
    // Đặt lại các giá trị ban đầu cho người chơi
    playerRow = 1;
    playerCol = 1;
    blood = 500;
    score = 0;
    breakCount = 5;
    hiddenCount = 0;
    gameFinished = false;
    gameQuit = false;
    // Bắt đầu lại đồng hồ
    myTimer.start();
    // Đặt lại vị trí của người chơi trong mê cung
    StartGame(); // Vị trí ban đầu của người chơi
}

  // Vẽ viên đạn lên màn hình
void Player::renderBullet() {
    if (bullet.isActive()) {
        // Lấy vị trí của viên đạn
        int bulletX = bullet.getCol() * CELL_SIZE;
        int bulletY = bullet.getRow() * CELL_SIZE;

         SDL_SetRenderDrawColor(renderer, 255, 0, 255, 255); // Màu hồng
        SDL_Rect bulletRect = { bulletX, bulletY, 10, 5 }; // Thay thế BULLET_WIDTH và BULLET_HEIGHT bằng kích thước thực của viên đạn
        SDL_RenderFillRect(renderer, &bulletRect);
    }
}
