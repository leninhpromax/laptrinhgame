#include <iostream>
#include "man_hinh.h"

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
    SDL_Texture* player = loadTexture("_98726804-69ea-42a3-8512-b3986c6ab364.jpg", renderer);
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

    // Mảng chứa thông tin về các tường: x, y, w, h
    SDL_Rect walls[5] = {
        {100, 100, 50, 200},
        {200, 200, 100, 50},
        {400, 300, 150, 50},
        {500, 400, 50, 150},
        {300, 100, 50, 150}
    };

    // Vẽ hình nền lên renderer
    SDL_RenderCopy(renderer, background, NULL, NULL);

    // Vẽ các tường lên renderer
    SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
    for (int i = 0; i < 5; ++i) {
        SDL_RenderFillRect(renderer, &walls[i]);
    }

    // Vẽ nhân vật người chơi và mục tiêu lên renderer
    renderTexture(player, 50, 50, 20, 20, renderer); // Đặt kích thước của người chơi là 20x20
    renderTexture(target, 700, 500, 20, 20, renderer); // Đặt kích thước của mục tiêu là 20x20

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
