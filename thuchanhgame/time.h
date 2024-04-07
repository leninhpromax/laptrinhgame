#ifndef TIMER_H
#define TIMER_H

#include "man_hinh.h" // Sử dụng để có thể sử dụng các hằng số và hàm trong file man_hinh.h
#include <iostream> // Thư viện nhập xuất cơ bản
#include <string> // Thư viện chuỗi
#include <sstream> // Thư viện chuỗi stringstream
#include <SDL_ttf.h> // Thư viện SDL_ttf để làm việc với văn bản trong SDL
#include <SDL.h> // Thư viện SDL

extern TTF_Font* font; // Biến font được sử dụng bên ngoài, được khai báo ở file main.cpp
// Đưa biến font vào phạm vi toàn cục để có thể sử dụng ở các file khác

extern SDL_Color textColor; // Màu chữ textColor được sử dụng bên ngoài, được khai báo ở file main.cpp
// Đưa màu textColor vào phạm vi toàn cục để có thể sử dụng ở các file khác

class Timer {
private:
    int startTicks; // Thời điểm bắt đầu đếm
    int pausedTicks; // Thời lượng đã tạm dừng
    bool paused; // Đánh dấu đã tạm dừng
    bool started; // Đánh dấu đã bắt đầu
public:
    Timer(); // Hàm khởi tạo

    void start(); // Bắt đầu đếm thời gian
    void stop(); // Dừng đếm thời gian
    void pause(); // Tạm dừng đếm thời gian
    void unpause(); // Tiếp tục đếm thời gian sau khi tạm dừng
    int get_ticks(); // Lấy thời gian đã trôi qua tính theo miligiây
    bool is_started(); // Kiểm tra xem đã bắt đầu đếm thời gian chưa
    bool is_paused(); // Kiểm tra xem đang trong trạng thái tạm dừng hay không
};

void renderText(SDL_Renderer* renderer, TTF_Font* font, const char* text, int x, int y);
// Khai báo hàm renderText để hiển thị văn bản lên renderer

#endif // TIMER_H
