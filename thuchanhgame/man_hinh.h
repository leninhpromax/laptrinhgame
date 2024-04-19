#ifndef MAN_HINH_H
#define MAN_HINH_H

#include <SDL.h>
#include <SDL_image.h>
#include <SDL_ttf.h>
#include <iostream>

struct WindowConfig {
    int width;
    int height;
    const char* title;
};

const WindowConfig WINDOW_CONFIG = { 1200, 800, "Game Window" };

extern SDL_Color textColor; // Màu chữ textColor được sử dụng bên ngoài, được khai báo ở file main.cpp

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

// Khai báo hàm renderText để hiển thị văn bản lên renderer
void renderText(SDL_Renderer* renderer, TTF_Font* font, const char* text, int x, int y);

#endif // MAN_HINH_H
