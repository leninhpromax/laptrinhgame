#ifndef SPRITE_H
#define SPRITE_H

struct Sprite {
    SDL_Texture* texture;
    std::vector<SDL_Rect> clips;
    int currentFrame = 0;
    void init(SDL_Texture* _texture, int frames, const int _clips [][4]) {
        texture = _texture;

        SDL_Rect clip;
        for (int i = 0; i < frames; i++) {
            clip.x = _clips[i][0];
            clip.y = _clips[i][1];
            clip.w = _clips[i][2];
            clip.h = _clips[i][3];
            clips.push_back(clip);
        }
    }
    void tick() {
        currentFrame = (currentFrame + 1) % clips.size();
    }

    const SDL_Rect* getCurrentClip() const {
        return &(clips[currentFrame]);
    }
};
//di chuyển sang trái
const int MOVELEFT[][4] = {
    { 128, 0, 192, 330},
    { 320, 0, 256, 330},
};
const int FRAME1 = sizeof(MOVELEFT) / sizeof(int)/4;
const int MOVELEFTA[][4] = {
    { 576, 0, 256, 330},
    { 576, 347, 256, 330},
    { 128, 704, 235, 330},
    { 832, 704, 192, 330},
};
const int FRAME2 = sizeof(MOVELEFTA) / sizeof(int)/4;
const int MOVELEFTB[][4] = {
    { 363, 347, 214, 330},
    { 128, 347, 235, 330},
};
const int FRAME3 = sizeof(MOVELEFTB) / sizeof(int)/4;
const int MOVELEFTC[][4] = {
    { 832, 347, 192, 330},
};
const int FRAME4 = sizeof(MOVELEFTC) / sizeof(int)/4;
//di chuyển lên trên
const int MOVEUP[][4] = {
    { 832, 0, 192, 330},
    { 363, 347, 214, 330},
};
const int FRAME5 = sizeof(MOVEUP) /sizeof(int)/4;
//di chuyển sang phải
const int MOVERIGHT[][4] = {
    { 704, 0, 192, 330},
    { 448, 0, 256, 330},
};
const int FRAME6 = sizeof(MOVERIGHT) / sizeof(int)/4;
const int MOVERIGHTA[][4] = {
    { 192, 0, 256, 330},
    { 192, 347, 256, 330},
    { 704, 704, 235, 330},
    { 0, 704, 192, 330},
};
const int FRAME7 = sizeof(MOVERIGHTA) / sizeof(int)/4;
const int MOVERIGHTB[][4] = {
    { 448, 347, 214, 330},
    { 704, 347, 235, 330},
};
const int FRAME8 = sizeof(MOVERIGHTB) / sizeof(int)/4;
const int MOVERIGHTC[][4] = {
    { 0, 347, 192, 330},
};
const int FRAME9 = sizeof(MOVERIGHTC) / sizeof(int)/4;
//di chuyển xuống dưới
const int MOVEDOWN[][4] = {
    { 0, 0, 192, 330},
    { 448, 347, 214, 330},
};
const int FRAME10 = sizeof(MOVEDOWN) / sizeof(int)/4;
#endif
