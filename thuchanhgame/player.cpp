#include "player.h"

Player::Player(Timer& timer, Maze& maze, Bullet& bullet) : myTimer(timer), mazeG(maze), bullet(bullet) {
    playerRow = 1;
    playerCol = 1;
    blood = 500;
    score = 0;
    breakCount = 5;
    hiddenCount = 0;
    run = 0;
    win = false;
    gameFinished = false;
    gameQuit = false;
    restart = false;
    movedUp = false;
    movedDown = false;
    movedLeft = false;
    movedRight = false;
    newgame = true;
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
    if (!isGameQuit() ) {
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
                        if (!bullet.IsActive()) {
                            bullet.Shoot(playerRow, playerCol, movedUp, movedDown, movedLeft, movedRight);
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
    renderBullet();
}

void Player::RenderMaze() {
    int widthl = 800; // Kích thước màn hình là 1200x800
    int heightl = 800;
    int MAZE_SIZE = 100; // Kích thước mê cung

    SDL_SetRenderDrawColor(renderer, 0, 255, 0, 255); // Màu xanh lá (R = 0, G = 255, B = 0)
    SDL_Rect greenRect = {0, 0, 800, 800};
    SDL_RenderFillRect(renderer, &greenRect);
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
                    renderTexture(target4, x, y, CELL_SIZE, CELL_SIZE, renderer);
                    break;
                case 3: // Nơi ẩn
                    renderTexture(target, x, y, CELL_SIZE, CELL_SIZE, renderer);
                    break;
                case 4: // Cổng bí mật
                    SDL_SetRenderDrawColor(renderer, 255, 0, 255, 255);
                    SDL_RenderFillRect(renderer, &rect);
                    break;
                case 5: // Vị trí người chơi
                    renderTexture(player, x, y, CELL_SIZE, CELL_SIZE, renderer);
                    break;
                case 6:
                    renderTexture(target2, x, y, CELL_SIZE, CELL_SIZE, renderer);
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
                    renderTexture(target4, x, y, CELL_SIZE, CELL_SIZE, renderer);
                    break;
                case 3: // Nơi ẩn
                    renderTexture(target, x, y, CELL_SIZE, CELL_SIZE, renderer);
                    break;
                case 4: // Cổng bí mật
                    SDL_SetRenderDrawColor(renderer, 255, 0, 255, 255);
                    SDL_RenderFillRect(renderer, &rect);
                    break;
                case 5: // Vị trí người chơi
                    renderTexture(player, x, y, CELL_SIZE, CELL_SIZE, renderer);
                    break;
                case 6:
                    renderTexture(target2, x, y, CELL_SIZE, CELL_SIZE, renderer);
                    break;
                default:
                    break;
            }
        }
    }
}
}
if (isReStart()){
    Uint32 restartTime = SDL_GetTicks();
        bool isReadyDisplayed = false;
        bool isCountdownStarted = false;

        while (SDL_GetTicks() - restartTime <= 4000) {
            if (!isReadyDisplayed && SDL_GetTicks() - restartTime >= 1000) {
                SDL_RenderClear(renderer);
                SDL_RenderCopy(renderer, background, NULL, NULL);

                std::stringstream readyText;
                readyText << "Ready";
                renderText(renderer, font1, readyText.str().c_str(), 400, 350, blueColor);
                SDL_RenderPresent(renderer);
                isReadyDisplayed = true;
            } else if (!isCountdownStarted && SDL_GetTicks() - restartTime >= 2000) {
                isCountdownStarted = true;
            } else if (isCountdownStarted) {
                SDL_RenderClear(renderer);
                SDL_RenderCopy(renderer, background, NULL, NULL);
                int countdown = (4000 - (SDL_GetTicks() - restartTime)) / 1000 + 1;

                std::stringstream countdownText;
                countdownText << countdown;
                renderText(renderer, font1, countdownText.str().c_str(), 450, 350, blueColor);
                SDL_RenderPresent(renderer);
            }

            if (isReadyDisplayed && isCountdownStarted && SDL_GetTicks() - restartTime >= 4000) {
                setReStart(false);
                myTimer.start();
            }
        }
}
// Hiển thị hướng dẫn khi game đang tạm dừng
    if (isGameQuit() && !isReStart()) {
        SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
        SDL_Rect whiteRect = {125, 350, 550, 100};
        SDL_RenderFillRect(renderer, &whiteRect);

        std::stringstream instructionText;
        instructionText << "Press Q to Continue or Press R to Restart";
        renderText(renderer, font, instructionText.str().c_str(), 150, 400, blueColor);
    }

    // Hiển thị thời gian, điểm, và thông tin khác

   std::stringstream runText;
    runText << "Run: " << run;
    renderText(renderer, font, runText.str().c_str(), 850, 600, textColor);

    std::stringstream timeText;
    timeText << "Timer: " << static_cast<int>(myTimer.get_ticks() / 1000.f);
    renderText(renderer, font, timeText.str().c_str(), 850, 100, textColor);

    std::stringstream bloodText;
    bloodText << "Blood: " << blood;
    renderText(renderer, font, bloodText.str().c_str(), 850, 200, textColor);

    std::stringstream bulletText;
    bulletText << "Bullet: " << bullet.GetBulletCount();
    renderText(renderer, font, bulletText.str().c_str(), 850, 600, textColor);

    std::stringstream scoreText;
    scoreText << "Score: " << score;
    renderText(renderer, font, scoreText.str().c_str(), 850, 300, textColor);

    std::stringstream hiddenCountText;
    hiddenCountText << "hiddenCount: " << hiddenCount;
    renderText(renderer, font, hiddenCountText.str().c_str(), 850, 400, textColor);

    std::stringstream breakCountText;
    breakCountText << "breakCount: " << breakCount;
    renderText(renderer, font, breakCountText.str().c_str(), 850, 500, textColor);

    SDL_RenderPresent(renderer); // Hiển thị lên màn hình
}

void Player::WinnerScreen() {
    if (win == true){
    SDL_RenderClear(renderer);
    SDL_RenderCopy(renderer, background2, NULL, NULL);

    // Tính thời gian đã chơi
    int TimeInSeconds = myTimer.get_ticks() / 1000.f;

    std::stringstream winText;
    winText << "Win" ;
    renderText(renderer, font1, winText.str().c_str(), 400, 350, redColor);

    // Hiển thị điểm số và thời gian chơi
    std::stringstream scoreText;
    scoreText << "Your score: " << score << " seconds";
    renderText(renderer, font, scoreText.str().c_str(), 400, 600, redColor);

    std::stringstream timeText;
    timeText << "Time: " << TimeInSeconds << " seconds";
    renderText(renderer, font, timeText.str().c_str(), 400, 500, redColor);
    std::stringstream runText;
    runText << "Run: " << run;
    renderText(renderer, font, runText.str().c_str(), 400, 700, redColor);

    // Hiển thị lên màn hình
    SDL_RenderPresent(renderer);
    }
    else {
    SDL_RenderClear(renderer);
    SDL_RenderCopy(renderer, background2, NULL, NULL);

    // Tính thời gian đã chơi
    int TimeInSeconds = myTimer.get_ticks() / 1000.f;

    std::stringstream lossText;
    lossText << "Loss" ;
    renderText(renderer, font1, lossText.str().c_str(), 400, 350, redColor);

    // Hiển thị điểm số và thời gian chơi
    std::stringstream scoreText;
    scoreText << "Your score: " << score << " seconds";
    renderText(renderer, font, scoreText.str().c_str(), 400, 600, redColor);

    std::stringstream timeText;
    timeText << "Time: " << TimeInSeconds << " seconds";
    renderText(renderer, font, timeText.str().c_str(), 400, 500, redColor);
    std::stringstream runText;
    runText << "Run: " << run;
    renderText(renderer, font, runText.str().c_str(), 400, 700, redColor);

    // Hiển thị lên màn hình
    SDL_RenderPresent(renderer);
    }
}

void Player::Moveup() {
    run ++;
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
                int hidden = rand() % 5 + 1;
                if (hidden == 1) {
                    score++;
                } else if (hidden == 2) {
                    breakCount++;
                } else if (hidden == 3) {
                    int mau = (rand() % 10 + 1) * 10;
                    blood -= mau;
                } else if (hidden == 4) {
                    bullet.plusBulletCount();
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
        }
        else if(nextCell == 6){
            win = true;
            setGameFinished(true);
        } else { // Nếu người chơi chạm vào tường hoặc hết máu
            setGameFinished(true);
        }
    } else { // Nếu người chơi ở hàng trên cùng của mê cung
        setGameFinished(true); // Kết thúc trò chơi
    }
}

void Player::Movedown() {
    run ++;
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
                int hidden = rand() % 5 + 1;
                if (hidden == 1) {
                    score++;
                } else if (hidden == 2) {
                    breakCount++;
                } else if (hidden == 3) {
                    int mau = (rand() % 10 + 1) * 10;
                    blood -= mau;
                } else if (hidden == 4) {
                    bullet.plusBulletCount();
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
    run ++;
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
                int hidden = rand() % 5 + 1;
                if (hidden == 1) {
                    score++;
                } else if (hidden == 2) {
                    breakCount++;
                } else if (hidden == 3) {
                    int mau = (rand() % 10 + 1) * 10;
                    blood -= mau;
                } else if (hidden == 4) {
                    bullet.plusBulletCount();
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
    run ++;
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
                int hidden = rand() % 5 + 1;
                if (hidden == 1) {
                    score++;
                } else if (hidden == 2) {
                    breakCount++;
                } else if (hidden == 3) {
                    int mau = (rand() % 10 + 1) * 10;
                    blood -= mau;
                } else if (hidden == 4) {
                    bullet.plusBulletCount();
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
    run = 0;
    gameFinished = false;
    gameQuit = false;
    restart = true;
    newgame = true;
    // Đặt lại vị trí của người chơi trong mê cung
    StartGame(); // Vị trí ban đầu của người chơi
}

 // Vẽ viên đạn lên màn hình
void Player::renderBullet() {
    if (bullet.IsActive()) {
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
                        if (i == bullet.GetRow() && j == bullet.GetCol()){
                            // Vẽ viên đạn lên màn hình
                            SDL_Rect bulletRect;
                            if (hasMovedDown() || hasMovedUp()) {
                                SDL_SetRenderDrawColor(renderer, 255, 0, 255, 255); // Màu hồng
                                bulletRect = { x + 5, y, 5, 10 };
                            } else if (hasMovedLeft() || hasMovedRight()) {
                                SDL_SetRenderDrawColor(renderer, 255, 0, 255, 255); // Màu hồng
                                bulletRect = { x, y + 5, 10, 5 };
                            }
                            SDL_RenderFillRect(renderer, &bulletRect);
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
                        if (mazeX == bullet.GetCol() && mazeY == bullet.GetRow()){
                            // Vẽ viên đạn lên màn hình
                            SDL_Rect bulletRect;
                            if (hasMovedDown() || hasMovedUp()) {
                                SDL_SetRenderDrawColor(renderer, 255, 0, 255, 255); // Màu hồng
                                bulletRect = { x + 5, y, 5, 10 };
                            } else if (hasMovedLeft() || hasMovedRight()) {
                                SDL_SetRenderDrawColor(renderer, 255, 0, 255, 255); // Màu hồng
                                bulletRect = { x, y + 5, 10, 5 };
                            }
                            SDL_RenderFillRect(renderer, &bulletRect);
                        }
                    }
                }
            }
        }
    }
}


void Player::AskForContinue() {
    SDL_RenderClear(renderer);
    SDL_RenderCopy(renderer, background2, NULL, NULL);

    // Hiển thị thông báo yêu cầu người chơi chọn
    SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
    SDL_Rect whiteRect = {325, 300, 550, 100}; // Cập nhật tọa độ cho màn hình 1200x800
    SDL_RenderFillRect(renderer, &whiteRect);

    std::stringstream continueText;
    continueText << "Do you want to continue? (Y/N)";
    renderText(renderer, font, continueText.str().c_str(), 450, 350, redColor); // Cập nhật tọa độ cho màn hình 1200x800

    // Hiển thị phím Yes và No
    renderText(renderer, font, "Yes", 400, 450, redColor); // Cập nhật tọa độ cho màn hình 1200x800
    renderText(renderer, font, "No", 650, 450, redColor);  // Cập nhật tọa độ cho màn hình 1200x800

    SDL_RenderPresent(renderer);

    bool continueLoop = true; // Biến để tiếp tục vòng lặp

    while (continueLoop) {
        SDL_Event e;
        while (SDL_PollEvent(&e) != 0) {
            if (e.type == SDL_QUIT) {
                // Thoát khỏi vòng lặp nếu người dùng tắt cửa sổ
                continueLoop = false;
            }
            else if (e.type == SDL_MOUSEBUTTONDOWN) {
                int mouseX, mouseY;
                SDL_GetMouseState(&mouseX, &mouseY);
                // Kiểm tra xem chuột được nhấn trong hộp chữ trắng hay không
                if (mouseX >= 325 && mouseX <= 875 && mouseY >= 300 && mouseY <= 400) {
                    if (e.button.button == SDL_BUTTON_LEFT) {
                        // Xử lý sự kiện khi người chơi nhấn chuột trái
                        if (mouseX <= 600) {
                            restartGame();
                            setReStart(false);
                            setNewgame(false);
                            continueLoop = false;
                        } else {
                            setNewgame(true);
                            continueLoop = false; // Thoát khỏi vòng lặp
                        }
                        continueLoop = false; // Thoát khỏi vòng lặp
                    }
                }
            }
            else if (e.type == SDL_KEYDOWN) {
                switch (e.key.keysym.sym) {
                    case SDLK_y:
                        // Bắt đầu lại trò chơi nếu nhấn 'Y'
                        restartGame();
                        setReStart(false);
                        setNewgame(false);
                        continueLoop = false; // Thoát khỏi vòng lặp
                        break;
                    case SDLK_n:
                        // Chọn không tiếp tục nếu nhấn 'N'
                        setNewgame(true);
                        continueLoop = false; // Thoát khỏi vòng lặp
                        break;
                }
            }
        }
    }
}
