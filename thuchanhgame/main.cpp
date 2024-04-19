#include <iostream>
#include <SDL.h>
#include <SDL_image.h>
#include <SDL_ttf.h>
#include "man_hinh.h"
#include "code_me_cung.h"
#include "time.h"
#include "player.h"

int main(int argc, char* argv[]) {
    // Khởi tạo SDL và tạo cửa sổ
    SDL_Window* window = initSDL(WINDOW_CONFIG.width, WINDOW_CONFIG.height, WINDOW_CONFIG.title);
    if (window == nullptr) {
        std::cerr << "Failed to create window! SDL Error: " << SDL_GetError() << std::endl;
        return 1;
    }

    // Tạo renderer
    SDL_Renderer* renderer = createRenderer(window);
    if (renderer == nullptr) {
        std::cerr << "Failed to create renderer! SDL Error: " << SDL_GetError() << std::endl;
        SDL_DestroyWindow(window);
        return 1;
    }

    // Load hình nền
    SDL_Texture* background = loadTexture("OIG3.png", renderer);
    if (background == nullptr) {
        std::cerr << "Failed to load background texture! SDL Error: " << SDL_GetError() << std::endl;
        quitSDL(window, renderer);
        return 1;
    }

    // Load hình nhân vật người chơi
    SDL_Texture* playerTexture = loadTexture("player.jpg", renderer);
    if (playerTexture == nullptr) {
        std::cerr << "Failed to load player texture! SDL Error: " << SDL_GetError() << std::endl;
        quitSDL(window, renderer);
        return 1;
    }

    // Load hình mục tiêu
    SDL_Texture* targetTexture = loadTexture("congchua2.jpg", renderer);
    if (targetTexture == nullptr) {
        std::cerr << "Failed to load target texture! SDL Error: " << SDL_GetError() << std::endl;
        quitSDL(window, renderer);
        return 1;
    }

    // Load hình mục tiêu thứ 2
    SDL_Texture* target2Texture = loadTexture("congchua1.jpg", renderer);
    if (target2Texture == nullptr) {
        std::cerr << "Failed to load target2 texture! SDL Error: " << SDL_GetError() << std::endl;
        quitSDL(window, renderer);
        return 1;
    }

    // Khởi tạo SDL_ttf
    if (TTF_Init() == -1) {
        std::cerr << "SDL_ttf could not initialize! SDL_ttf Error: " << TTF_GetError() << std::endl;
        quitSDL(window, renderer);
        return 1;
    }

    // Load font
    TTF_Font* font = TTF_OpenFont("font.ttf", 32);
    if (font == nullptr) {
        std::cerr << "Failed to load font! SDL_ttf Error: " << TTF_GetError() << std::endl;
        quitSDL(window, renderer);
        TTF_Quit();
        return 1;
    }

    // Khởi tạo Timer và Maze
    Timer myTimer;
    Maze mazeG;
    Player playerG(myTimer, mazeG);
    // Bắt đầu trò chơi
    playerG.StartGame();

    bool quit = false;
    bool windowClosed = false; // Biến cờ để xác định liệu cửa sổ đã được đóng hay không

    // Xử lý sự kiện và vòng lặp chính
    while (!quit) {
        SDL_Event event;
        while (SDL_PollEvent(&event) != 0) {
            if (event.type == SDL_QUIT) {
                quit = true;
                windowClosed = true; // Đặt cờ thành true khi gặp sự kiện tắt cửa sổ
            }
        }

        // Kiểm tra cờ trước khi xử lý di chuyển
        if (!windowClosed) {
            // Vẽ hình nền lên renderer
            SDL_RenderCopy(renderer, background, NULL, NULL);
            playerG.RenderMaze(renderer, font, playerTexture, targetTexture, target2Texture);

            // Di chuyển người chơi
            playerG.Move();
            // Hiển thị renderer
            SDL_RenderPresent(renderer);
        }
        // Reset cờ để chuẩn bị cho vòng lặp tiếp theo
        windowClosed = false;
    }

    // Xử lý kết thúc trò chơi và hiển thị màn hình chiến thắng (nếu có)
    if (playerG.isGameFinished()) {
        playerG.WinnerScreen(renderer, font);
        myTimer.stop();
        SDL_Delay(2500); // Chờ 2,5 giây trước khi thoát chương trình
    }

    // Giải phóng bộ nhớ và đóng SDL
    SDL_DestroyTexture(background);
    SDL_DestroyTexture(playerTexture);
    SDL_DestroyTexture(target2Texture);
    SDL_DestroyTexture(targetTexture);
    quitSDL(window, renderer);
    TTF_CloseFont(font);
    TTF_Quit();

    return 0;
}
