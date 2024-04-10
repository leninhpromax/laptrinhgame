#ifndef PLAYER_H
#define PLAYER_H

#include <iostream>
#include <vector>
#include <SDL.h>
#include <SDL_image.h>
#include <SDL_ttf.h>
#include <sstream>
#include "time.h"
#include "code_me_cung.h"
#include "man_hinh.h"

class Player {
private:
    int playerRow;
    int playerCol;
    int blood;
    int score;
    int breakCount;
    int hiddenCount;
    bool gameFinished;
    bool gameQuit;
    Timer myTimer;
    Maze mazeG;
    SDL_Renderer* renderer;
    SDL_Texture* player;
    SDL_Texture* target;
    SDL_Texture* target2;

public:
    Player(Timer& timer, Maze& maze); // Constructor với Timer và Maze
    void StartGame();
    void Move();
    std::vector<std::vector<int>>& mazek(); // Sửa tên hàm mazek()

    int getRow() {
        return playerRow;
    }
    int getCol() {
        return playerCol;
    }
    int getBlood() {
        return blood;
    }
    int getScore() {
        return score;
    }
    int getBreakCount() {
        return breakCount;
    }
    int getHiddenCount() {
        return hiddenCount;
    }

    bool isGameFinished() {
        return gameFinished;
    }
    bool isGameQuit() {
        return gameQuit;
    }

    void setGameFinished(bool finished) {
        gameFinished = finished;
    }
    void setGameQuit(bool quit) {
        gameQuit = quit;
    }

    int& Replace(int row, int col);

    void RenderMaze(SDL_Renderer* renderer, TTF_Font* font, SDL_Texture* player, SDL_Texture* target, SDL_Texture* target2); // Loại bỏ tham số renderer
    void WinnerScreen(SDL_Renderer* renderer, TTF_Font* font);
};

#endif
