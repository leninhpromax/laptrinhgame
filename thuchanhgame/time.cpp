#include "time.h" // Sử dụng để có thể sử dụng các hàm thời gian (không dùng trong mã nguồn hiện tại)

TTF_Font* font = NULL; // Biến toàn cục để lưu trữ font chữ được sử dụng trong chương trình

SDL_Color textColor = { 255, 255, 255 }; // Màu chữ mặc định, sử dụng để hiển thị văn bản

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

void renderText(SDL_Renderer* renderer, TTF_Font* font, const char* text, int x, int y) {
    SDL_Surface* surface = TTF_RenderText_Solid(font, text, textColor); // Tạo surface chứa văn bản
    if (surface == nullptr) {
        std::cerr << "Unable to render text surface! SDL_ttf Error: " << TTF_GetError() << std::endl;
        return;
    }

    SDL_Texture* texture = SDL_CreateTextureFromSurface(renderer, surface); // Tạo texture từ surface
    SDL_FreeSurface(surface); // Giải phóng surface
    if (texture == nullptr) {
        std::cerr << "Unable to create texture from rendered text! SDL Error: " << SDL_GetError() << std::endl;
        return;
    }

    int texW = 0;
    int texH = 0;
    SDL_QueryTexture(texture, NULL, NULL, &texW, &texH); // Lấy kích thước của texture

    SDL_Rect destRect = { x, y, texW, texH }; // Tạo hình chữ nhật đích
    SDL_RenderCopy(renderer, texture, NULL, &destRect); // Sao chép texture lên renderer

    SDL_DestroyTexture(texture); // Giải phóng texture
}

