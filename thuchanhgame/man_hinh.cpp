#include "man_hinh.h"

// Màu chữ mặc định, sử dụng để hiển thị văn bản
SDL_Color textColor = { 255, 255, 255 };

// Màu chữ đỏ
SDL_Color redColor = { 255, 0, 0 };

// Màu chữ xanh
SDL_Color blueColor = { 0, 0, 255 };

void logErrorAndExit(const char* msg, const char* error) {
    SDL_LogMessage(SDL_LOG_CATEGORY_APPLICATION, SDL_LOG_PRIORITY_ERROR, "%s: %s", msg, error);
    SDL_Quit();
    exit(1); // Thoát khỏi chương trình với mã lỗi 1
}

SDL_Window* initSDL(int SCREEN_WIDTH, int SCREEN_HEIGHT, const char* WINDOW_TITLE) {
    // Khởi tạo SDL
    if (SDL_Init(SDL_INIT_EVERYTHING) != 0)
        logErrorAndExit("SDL_Init", SDL_GetError());

    // Tạo cửa sổ SDL
    SDL_Window* window = SDL_CreateWindow(WINDOW_TITLE, SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, SCREEN_WIDTH, SCREEN_HEIGHT, SDL_WINDOW_SHOWN);
    if (window == nullptr) logErrorAndExit("CreateWindow", SDL_GetError());

    // Khởi tạo SDL_image để có thể load các định dạng hình ảnh khác nhau
    if ((IMG_Init(IMG_INIT_PNG) & IMG_INIT_PNG) != IMG_INIT_PNG) // Kiểm tra chỉ cần SDL_IMAGE_INIT_PNG đã được khởi tạo
        logErrorAndExit("IMG_Init", IMG_GetError());

    return window;
}

SDL_Renderer* createRenderer(SDL_Window* window) {
    // Tạo renderer để vẽ đồ họa lên cửa sổ
    SDL_Renderer* renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);

    if (renderer == nullptr) logErrorAndExit("CreateRenderer", SDL_GetError());

    // Đặt chất lượng scale của renderer
    SDL_SetHint(SDL_HINT_RENDER_SCALE_QUALITY, "linear");
    // Đặt kích thước logic của renderer
    SDL_RenderSetLogicalSize(renderer, WINDOW_CONFIG.width, WINDOW_CONFIG.height);

    return renderer;
}

void quitSDL(SDL_Window* window, SDL_Renderer* renderer) {
    // Giải phóng bộ nhớ và đóng SDL
    IMG_Quit();
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    SDL_Quit();
}

void waitUntilKeyPressed() {
    // Chờ đến khi có một sự kiện xảy ra (phím được nhấn hoặc thoát chương trình)
    SDL_Event e;
    while (true) {
        if (SDL_PollEvent(&e) != 0) {
            if (e.type == SDL_KEYDOWN || e.type == SDL_QUIT) {
                return;
            }
        }
        SDL_Delay(100);
    }
}

void renderTexture(SDL_Texture *texture, int x, int y, int width, int height, SDL_Renderer* renderer) {
    if (texture == nullptr) return; // Kiểm tra texture có hợp lệ không trước khi vẽ

    // Vẽ một texture lên renderer tại vị trí (x, y) với kích thước (width, height)
    SDL_Rect dest;

    dest.x = x;
    dest.y = y;
    dest.w = width; // Thiết lập kích thước mới cho texture
    dest.h = height;
    SDL_RenderCopy(renderer, texture, NULL, &dest);
}

SDL_Texture* loadTexture(const char* filename, SDL_Renderer* renderer) {
    // Load một texture từ file và trả về
    SDL_LogMessage(SDL_LOG_CATEGORY_APPLICATION, SDL_LOG_PRIORITY_INFO, "Loading %s", filename);

    SDL_Texture* texture = IMG_LoadTexture(renderer, filename);
    if (texture == nullptr) {
        logErrorAndExit("LoadTexture", IMG_GetError());
    }

    return texture;
}

void renderText(SDL_Renderer* renderer, TTF_Font* font, const char* text, int x, int y, SDL_Color textColor) {
    // Tạo surface chứa văn bản
    SDL_Surface* surface = TTF_RenderText_Solid(font, text, textColor);
    if (surface == nullptr) {
        std::cerr << "Unable to render text surface! SDL_ttf Error: " << TTF_GetError() << std::endl;
        return;
    }

    // Tạo texture từ surface
    SDL_Texture* texture = SDL_CreateTextureFromSurface(renderer, surface);
    SDL_FreeSurface(surface); // Giải phóng surface sau khi tạo texture
    if (texture == nullptr) {
        std::cerr << "Unable to create texture from rendered text! SDL Error: " << SDL_GetError() << std::endl;
        return;
    }

    // Lấy kích thước của texture
    int texW = 0;
    int texH = 0;
    SDL_QueryTexture(texture, NULL, NULL, &texW, &texH);

    // Tạo hình chữ nhật đích
    SDL_Rect destRect = { x, y, texW, texH };

    // Sao chép texture lên renderer
    SDL_RenderCopy(renderer, texture, NULL, &destRect);

    // Giải phóng texture
    SDL_DestroyTexture(texture);
}
