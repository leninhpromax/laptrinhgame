#ifndef TIMER_H
#define TIMER_H

#include <iostream> // Thư viện nhập xuất cơ bản
#include <string> // Thư viện chuỗi
#include <SDL.h> // Thư viện SDL

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

#endif // TIMER_H
