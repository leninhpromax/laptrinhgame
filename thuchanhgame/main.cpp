#include <bits/stdc++.h>
#include <iostream>
#include <ctime>
#include "man_hinh.h"
#include "code_me_cung.h"
#include "time.h"

// Hàm main
int main(int argc, char *argv[]) {
  // Khởi tạo SDL và tạo cửa sổ
  SDL_Window* window = initSDL(SCREEN_WIDTH, SCREEN_HEIGHT, WINDOW_TITLE);
  // Tạo renderer
  SDL_Renderer* renderer = createRenderer(window);

  // Load hình nền
  SDL_Texture* background = loadTexture("OIG3.png", renderer);
  if (background == nullptr) {
    // Nếu load không thành công, giải phóng bộ nhớ và đóng SDL
    quitSDL(window, renderer);
    return 1;
  }

  // Load hình nhân vật người chơi
  SDL_Texture* player = loadTexture("player.jpg", renderer);
  if (player == nullptr) {
    // Nếu load không thành công, giải phóng bộ nhớ và đóng SDL
    quitSDL(window, renderer);
    return 1;
  }

  // Load hình mục tiêu
  SDL_Texture* target = loadTexture("congchua2.jpg", renderer);
  if (target == nullptr) {
    // Nếu load không thành công, giải phóng bộ nhớ và đóng SDL
    quitSDL(window, renderer);
    return 1;
  }

  // Load hình mục tiêu thứ 2
  SDL_Texture* target2 = loadTexture("congchua1.jpg", renderer);
  if (target2 == nullptr) {
    // Nếu load không thành công, giải phóng bộ nhớ và đóng SDL
    quitSDL(window, renderer);
    return 1;
  }

  if (TTF_Init() == -1) {
        std::cerr << "SDL_ttf could not initialize! SDL_ttf Error: " << TTF_GetError() << std::endl;
        quitSDL(window, renderer);
        return 1;
    }

  font = TTF_OpenFont("font.ttf", 28);
if (font == nullptr) {
    std::cerr << "Failed to load font! SDL_ttf Error: " << TTF_GetError() << std::endl;
    // Xử lý lỗi ở đây, có thể là thoát chương trình hoặc sử dụng một font mặc định khác
}

  // Khởi tạo mê cung và tạo các điểm thưởng, tường, cổng bí mật
  std::vector<std::vector<int>> maze(ROWS, std::vector<int>(COLUMNS, 0));
  srand(time(nullptr));
  CreateMaze(maze);
  GenerateRandomWalls(maze);
  FixMazeError(maze);
  FindRewards(maze);
  BreakWalls(maze);

  // Khởi tạo vị trí ban đầu của người chơi và điểm kết thúc
  int playerRow = 1;
  int playerCol = 1;
  maze[playerRow][playerCol] = 5;

  std::pair<int, int> endPosition = FindEmptySpace();
  int endRow = endPosition.first;
  int endCol = endPosition.second;
  maze[endRow][endCol] = 6;
  int score = 0;
  int breakCount = 5;
  int hiddenCount = 0;
  int blood = 500;
  bool kt = false;
   Timer myTimer;
   myTimer.start();
   bool pause = false;

  // Vẽ hình nền lên renderer
  SDL_RenderCopy(renderer, background, NULL, NULL);
  // Hiển thị mê cung và người chơi

  renderMaze(maze, player, target, target2, renderer, playerRow, playerCol);
  // Hiển thị lên màn hình
  SDL_RenderPresent(renderer);

  // Chương trình sẽ chạy cho đến khi người chơi chạm đến điểm kết thúc hoặc thoát chương trình
  while (kt == false) {
    // Vẽ hình nền lên renderer
    SDL_RenderCopy(renderer, background, NULL, NULL);
    renderMaze(maze, player, target, target2, renderer, playerRow, playerCol);

    // Di chuyển người chơi
    movePlayer(playerRow, playerCol, blood, score, breakCount, hiddenCount, maze, renderer, player, kt, pause, myTimer);
  }
  clearScreen(renderer, score, font, myTimer, SCREEN_WIDTH, SCREEN_HEIGHT);
  myTimer.stop();

  // Chờ 2,5 giây trước khi thoát chương trình
  SDL_Delay(2500);
  waitUntilKeyPressed();
  // Giải phóng bộ nhớ và đóng SDL
  SDL_DestroyTexture(player);
  SDL_DestroyTexture(target);
  SDL_DestroyTexture(background);
  SDL_DestroyTexture(target2);
  TTF_CloseFont(font);
  TTF_Quit();
  quitSDL(window, renderer);

  return 0;
}
