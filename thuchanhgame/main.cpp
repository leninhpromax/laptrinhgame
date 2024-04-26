
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
    TTF_Init();

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

    SDL_Texture* background1 = loadTexture("background1.jpg", renderer);
    if (background1 == nullptr) {
        std::cerr << "Failed to load background1 texture! SDL Error: " << SDL_GetError() << std::endl;
        quitSDL(window, renderer);
        return 1;
    }

    SDL_Texture* background2 = loadTexture("win.jpg", renderer);
    if (background2 == nullptr) {
        std::cerr << "Failed to load background2 texture! SDL Error: " << SDL_GetError() << std::endl;
        quitSDL(window, renderer);
        return 1;
    }

    // Load hình nhân vật người chơi
    SDL_Texture* player = loadTexture("playerx.png", renderer);
    if (player == nullptr) {
        std::cerr << "Failed to load player texture! SDL Error: " << SDL_GetError() << std::endl;
        quitSDL(window, renderer);
        return 1;
    }

    // Load hình mục tiêu
    SDL_Texture* target = loadTexture("rewards.png", renderer);
    if (target == nullptr) {
        std::cerr << "Failed to load target texture! SDL Error: " << SDL_GetError() << std::endl;
        quitSDL(window, renderer);
        return 1;
    }

    // Load hình mục tiêu thứ 2
    SDL_Texture* target2 = loadTexture("dich.png", renderer);
    if (target2 == nullptr) {
        std::cerr << "Failed to load target2 texture! SDL Error: " << SDL_GetError() << std::endl;
        quitSDL(window, renderer);
        return 1;
    }

     // Load hình mục tiêu thứ 3
    SDL_Texture* target3 = loadTexture("key.png", renderer);
    if (target3 == nullptr) {
        std::cerr << "Failed to load target3 texture! SDL Error: " << SDL_GetError() << std::endl;
        quitSDL(window, renderer);
        return 1;
    }

     SDL_Texture* target4 = loadTexture("tiennu.png", renderer);
    if (target4 == nullptr) {
        std::cerr << "Failed to load target4 texture! SDL Error: " << SDL_GetError() << std::endl;
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
    TTF_Font* font = TTF_OpenFont("font.ttf", 36);
    if (font == nullptr) {
        std::cerr << "Failed to load font! SDL_ttf Error: " << TTF_GetError() << std::endl;
        quitSDL(window, renderer);
        TTF_Quit();
        return 1;
    }

     TTF_Font* font1 = TTF_OpenFont("font.ttf", 60);
    if (font1 == nullptr) {
        std::cerr << "Failed to load font1! SDL_ttf Error: " << TTF_GetError() << std::endl;
        quitSDL(window, renderer);
        TTF_Quit();
        return 1;
    }

    // Khởi tạo Timer và Maze
    Timer myTimer;
    Maze mazeG;
    Player playerG(myTimer, mazeG);
    playerG.setRenderer(renderer);
    playerG.setFont(font);
    playerG.setFont1(font1);
    playerG.setBackground(background);
    playerG.setBackground2(background2);
    playerG.setPlayer(player);
    playerG.setTarget(target);
    playerG.setTarget2(target2);
    playerG.setTarget3(target3);
    playerG.setTarget4(target4);

    SDL_Event event;
bool kt = true;
SDL_RenderCopy(renderer, background1, NULL, NULL);
while (kt) {
    // Xử lý các sự kiện trong hàng đợi
    while (SDL_PollEvent(&event)) {
        if (event.type == SDL_MOUSEBUTTONDOWN && event.button.button == SDL_BUTTON_LEFT) {
            // Kiểm tra nếu là sự kiện nhấp chuột trái
            int mouseX, mouseY;
            SDL_GetMouseState(&mouseX, &mouseY);

            // Kiểm tra nếu vị trí chuột được nhấp nằm trong vùng bạn quan tâm
            if (mouseX >= 500 && mouseX <= 700 && mouseY >= 665 && mouseY <= 755) {
                kt = false;
        }
    }
    SDL_RenderPresent(renderer);
}
}
h:
if (!kt){
    // Bắt đầu trò chơi
    playerG.StartGame();
}

    while (!kt) {
    SDL_RenderCopy(renderer, background, NULL, NULL);
    playerG.RenderMaze();
    // Di chuyển người chơi
    playerG.Move();
    playerG.renderBullet();

    // Hiển thị renderer
    SDL_RenderPresent(renderer);
    kt = playerG.isGameFinished();
}
if (playerG.isGameFinished()) {
            playerG.WinnerScreen();
            myTimer.stop();
            SDL_Delay(2000);
             playerG.AskForContinue();
            if (!playerG.isNewGame()){
            kt = playerG.isNewGame();
            goto h;
            }
    }

SDL_Delay(2500); // Chờ 2,5 giây trước khi thoát chương trình
    // Giải phóng bộ nhớ và đóng SDL
    SDL_DestroyTexture(background);
    SDL_DestroyTexture(background1);
    SDL_DestroyTexture(background2);
    SDL_DestroyTexture(player);
    SDL_DestroyTexture(target2);
    SDL_DestroyTexture(target);
    SDL_DestroyTexture(target3);
    SDL_DestroyTexture(target4);
    quitSDL(window, renderer);
    TTF_CloseFont(font);
    TTF_CloseFont(font1);
    TTF_Quit();

    return 0;
}
