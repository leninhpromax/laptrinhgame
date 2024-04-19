#include "time.h" // Sử dụng để có thể sử dụng các hàm thời gian (không dùng trong mã nguồn hiện tại)

Timer::Timer() {
    startTicks = 0;
    pausedTicks = 0;
    paused = false;
    started = false;
}

void Timer::start() {
    started = true; // Bắt đầu đếm thời gian
    paused = false; // Không bị tạm dừng
    startTicks = SDL_GetTicks(); // Lưu thời điểm bắt đầu
}

void Timer::stop() {
    started = false; // Dừng đếm thời gian
    paused = false; // Không bị tạm dừng
}

void Timer::pause() {
    if (started && !paused) { // Nếu đang chạy và chưa tạm dừng
        paused = true; // Tạm dừng
        pausedTicks = SDL_GetTicks() - startTicks; // Lưu lại thời điểm tạm dừng
    }
}

void Timer::unpause() {
    if (paused) { // Nếu đã tạm dừng
        paused = false; // Không tạm dừng nữa
        startTicks = SDL_GetTicks() - pausedTicks; // Lấy lại thời điểm bắt đầu trừ đi thời gian tạm dừng
        pausedTicks = 0; // Đặt thời gian tạm dừng về 0
    }
}

int Timer::get_ticks() {
    if (started) { // Nếu đã bắt đầu
        if (paused) { // Nếu đang tạm dừng
            return pausedTicks; // Trả về thời gian đã tạm dừng
        } else {
            return SDL_GetTicks() - startTicks; // Trả về thời gian tính từ thời điểm bắt đầu
        }
    }
    return 0; // Nếu chưa bắt đầu, trả về 0
}

bool Timer::is_started() {
    return started; // Trả về true nếu đã bắt đầu, ngược lại false
}

bool Timer::is_paused() {
    return paused; // Trả về true nếu đang tạm dừng, ngược lại false
}
