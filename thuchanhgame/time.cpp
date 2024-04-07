#include "time.h"
#include <iostream>

SDL_Event event1; // Khởi tạo event1

TTF_Font* font = NULL; // Khởi tạo font

SDL_Color textColor = { 255, 255, 255 }; // Khởi tạo màu textColor

Timer::Timer() {
    startTicks = 0;
    pausedTicks = 0;
    paused = false;
    started = false;
}

void Timer::start() {
    started = true;
    paused = false;
    startTicks = SDL_GetTicks();
}

void Timer::stop() {
    started = false;
    paused = false;
}

void Timer::pause() {
    if (started && !paused) {
        paused = true;
        pausedTicks = SDL_GetTicks() - startTicks;
    }
}

void Timer::unpause() {
    if (paused) {
        paused = false;
        startTicks = SDL_GetTicks() - pausedTicks;
        pausedTicks = 0;
    }
}

int Timer::get_ticks() {
    if (started) {
        if (paused) {
            return pausedTicks;
        } else {
            return SDL_GetTicks() - startTicks;
        }
    }
    return 0;
}

bool Timer::is_started() {
    return started;
}

bool Timer::is_paused() {
    return paused;
}

void renderText(SDL_Renderer* renderer, TTF_Font* font, const char* text, int x, int y) {
    SDL_Surface* surface = TTF_RenderText_Solid(font, text, textColor);
    if (surface == nullptr) {
        std::cerr << "Unable to render text surface! SDL_ttf Error: " << TTF_GetError() << std::endl;
        return;
    }

    SDL_Texture* texture = SDL_CreateTextureFromSurface(renderer, surface);
    SDL_FreeSurface(surface);
    if (texture == nullptr) {
        std::cerr << "Unable to create texture from rendered text! SDL Error: " << SDL_GetError() << std::endl;
        return;
    }

    int texW = 0;
    int texH = 0;
    SDL_QueryTexture(texture, NULL, NULL, &texW, &texH);

    SDL_Rect destRect = { x, y, texW, texH };
    SDL_RenderCopy(renderer, texture, NULL, &destRect);

    SDL_DestroyTexture(texture);
}
