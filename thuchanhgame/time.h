#ifndef TIMER_H
#define TIMER_H

#include "man_hinh.h"
#include <iostream>
#include <string>
#include <sstream>
#include <SDL_ttf.h>

extern SDL_Event event1; // Đưa event1 vào phạm vi toàn cục

extern TTF_Font* font; // Đưa biến font vào phạm vi toàn cục

extern SDL_Color textColor; // Đưa màu textColor vào phạm vi toàn cục

class Timer {
private:
    int startTicks;
    int pausedTicks;
    bool paused;
    bool started;
public:
    Timer();

    void start();
    void stop();
    void pause();
    void unpause();
    int get_ticks();
    bool is_started();
    bool is_paused();
};

void renderText(SDL_Renderer* renderer, TTF_Font* font, const char* text, int x, int y); // Khai báo hàm renderText

#endif // TIMER_H
