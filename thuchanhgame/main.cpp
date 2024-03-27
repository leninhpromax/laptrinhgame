#include <iostream>
#include "man_hinh.h"
#include "code_me_cung.h"

// Khai báo hàm mới để vẽ mê cung
void renderMaze(std::vector<std::vector<int>>& maze, int endRow, int endCol, SDL_Texture* player, SDL_Texture* target,SDL_Texture* target2, SDL_Renderer* renderer) {

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
     SDL_Texture* target2 = loadTexture("congchua1.jpg", renderer);
    if (target == nullptr) {
        // Nếu load không thành công, giải phóng bộ nhớ và đóng SDL
        quitSDL(window, renderer);
        return 1;
    }

    std::vector<std::vector<int>> maze(ROWS, std::vector<int>(COLUMNS, 0));
    srand(time(nullptr));
    CreateMaze(maze);
    GenerateRandomWalls(maze);
    FixMazeError(maze);
    FindRewards(maze);
    BreakWalls(maze);

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

    // Gọi hàm để vẽ mê cung
    renderMaze(maze, endRow, endCol, player, target, target2, renderer);

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
