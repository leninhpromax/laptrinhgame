#include <iostream>
#include "man_hinh.h"
#include "code_me_cung.h"

int main(int argc, char *argv[])
{
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
    SDL_Texture* target = loadTexture("_0af7a7fc-de20-4425-a534-5b36c221ea87.jpg", renderer);
    if (target == nullptr) {
        // Nếu load không thành công, giải phóng bộ nhớ và đóng SDL
        quitSDL(window, renderer);
        return 1;
    }

    std::vector<std::vector<int>> maze(ROWS, std::vector<int>(COLUMNS, 0));
    srand(time(nullptr));

    CreateMaze();
    GenerateRandomWalls();
    FixMazeError();
    FindRewards();
    BreakWalls();

    int playerRow = 1;
    int playerCol = 1;
    int score = 0;
    int breakCount = 3;
    int hiddenCount = 0;
    maze[playerRow][playerCol] = 5;

    std::pair<int, int> endPosition = FindEmptySpace();
    int endRow = endPosition.first;
    int endCol = endPosition.second;

    maze[endRow][endCol] = 6;

    // Vẽ hình nền lên renderer
    SDL_RenderCopy(renderer, background, NULL, NULL);

   for (int i = 0; i < ROWS; i++) {
    for (int j = 0; j < COLUMNS; j++) {
        if (maze[i][j] == 5) {
            renderTexture(player, 16 * i, 16 * j, 16, 16, renderer);
        } else if (maze[i][j] == 6) {
            renderTexture(target, 16 * i, 16 * j, 16, 16, renderer);
        } else if (maze[i][j] == 0) { // Điều kiện mới để in ra tường
            SDL_Rect wallRect = {16 * i, 16 * j, 16, 16}; // Tạo hình chữ nhật cho tường
            SDL_SetRenderDrawColor(renderer, 255, 0, 0, 255); // Đặt màu cho tường (màu đỏ)
            SDL_RenderFillRect(renderer, &wallRect); // Vẽ tường lên renderer
        }
    }
}
    // Hiển thị toàn bộ nội dung đã vẽ trên renderer lên màn hình
    SDL_RenderPresent(renderer);

    // Chờ đến khi có một phím được nhấn hoặc thoát chương trình
    waitUntilKeyPressed();

    // Giải phóng bộ nhớ và đóng SDL
    SDL_DestroyTexture(player);
    SDL_DestroyTexture(target);
    SDL_DestroyTexture(background);
    quitSDL(window, renderer);
    return 0;
}
