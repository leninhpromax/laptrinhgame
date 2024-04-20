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

    // Load hình nhân vật người chơi
    SDL_Texture* player = loadTexture("player.jpg", renderer);
    if (player == nullptr) {
        std::cerr << "Failed to load player texture! SDL Error: " << SDL_GetError() << std::endl;
        quitSDL(window, renderer);
        return 1;
    }

    // Load hình mục tiêu
    SDL_Texture* target = loadTexture("congchua2.jpg", renderer);
    if (target == nullptr) {
        std::cerr << "Failed to load target texture! SDL Error: " << SDL_GetError() << std::endl;
        quitSDL(window, renderer);
        return 1;
    }

    // Load hình mục tiêu thứ 2
    SDL_Texture* target2 = loadTexture("congchua1.jpg", renderer);
    if (target2 == nullptr) {
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
    playerG.setRenderer(renderer);
    playerG.setFont(font);
    playerG.setPlayer(player);
    playerG.setTarget(target);
    playerG.setTarget2(target2);
    // Bắt đầu trò chơi
    playerG.StartGame();
    bool kt = false;

    while (!kt) {
    SDL_RenderCopy(renderer, background, NULL, NULL);
    playerG.RenderMaze();
    // Di chuyển người chơi
    playerG.Move();
    // Hiển thị renderer
    SDL_RenderPresent(renderer);
    kt = playerG.isGameFinished();
    std::cout << playerG.isGameFinished() << std:: endl;

}
if (playerG.isGameFinished()) {
        playerG.WinnerScreen();
        myTimer.stop();
        SDL_Delay(2500); // Chờ 2,5 giây trước khi thoát chương trình
    }

    // Giải phóng bộ nhớ và đóng SDL
    SDL_DestroyTexture(background);
    SDL_DestroyTexture(player);
    SDL_DestroyTexture(target2);
    SDL_DestroyTexture(target);
    quitSDL(window, renderer);
    TTF_CloseFont(font);
    TTF_Quit();

    return 0;
}
