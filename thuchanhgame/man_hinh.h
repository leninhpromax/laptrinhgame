#ifndef GAME_H
#define GAME_H

#include <SDL.h>
#include <SDL_image.h>
#include <iostream>

// Khai báo các biến toàn cục
extern const int SCREEN_WIDTH;        // Chiều rộng của cửa sổ
extern const int SCREEN_HEIGHT;       // Chiều cao của cửa sổ
extern const char* WINDOW_TITLE;      // Tiêu đề của cửa sổ

// Hàm để ghi log lỗi và thoát khỏi SDL
void logErrorAndExit(const char* msg, const char* error);
// Hàm để khởi tạo SDL và cửa sổ
SDL_Window* initSDL(int SCREEN_WIDTH, int SCREEN_HEIGHT, const char* WINDOW_TITLE);
// Hàm để tạo renderer để vẽ đồ họa lên cửa sổ
SDL_Renderer* createRenderer(SDL_Window* window);
// Hàm để giải phóng bộ nhớ và đóng SDL
void quitSDL(SDL_Window* window, SDL_Renderer* renderer);
// Hàm để chờ đến khi có một phím được nhấn hoặc thoát chương trình
void waitUntilKeyPressed();
// Hàm để vẽ một texture lên renderer với kích thước được chỉ định
void renderTexture(SDL_Texture *texture, int x, int y, int width, int height, SDL_Renderer* renderer);
// Hàm để load một texture từ file và trả về
SDL_Texture *loadTexture(const char *filename, SDL_Renderer* renderer);

#endif // GAME_H
