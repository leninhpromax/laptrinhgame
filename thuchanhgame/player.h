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
#include "bullet.h"

const int VIEW_SIZE = 40; // Kích thước màn hình 40x40 ô
const int CELL_SIZE = 20; // Kích thước của mỗi ô

class Player {
private:
    int playerRow;
    int playerCol;
    int blood;
    int score;
    int breakCount;
    int hiddenCount;
    int run;
    bool win;
    bool gameFinished;
    bool restart;
    bool gameQuit;
    bool movedUp;
    bool movedDown;
    bool movedLeft;
    bool movedRight;
    bool newgame;
    bool key;
    Timer myTimer;
    Maze mazeG;
    Bullet bullet;

    TTF_Font* font;
    TTF_Font* font1;
    SDL_Renderer* renderer;
    SDL_Texture* background;
    SDL_Texture* background2;
    SDL_Texture* player;
    SDL_Texture* target;
    SDL_Texture* target2;
    SDL_Texture* target3;
    SDL_Texture* target4;

public:
    Player(Timer& timer, Maze& maze, Bullet& bullet); // Constructor với Timer và Maze
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

    bool isReStart() {
        return restart;
    }

    bool isWin() {
        return win;
    }

    bool isNewGame() {
        return newgame;
    }

    void setNewgame(bool ng) {
        newgame = ng;
    }

    void setWin(bool w) {
        win = w;
    }

    void setReStart(bool reS) {
        restart = reS;
    }

    void setGameFinished(bool finished) {
        gameFinished = finished;
    }
    void setGameQuit(bool quit) {
        gameQuit = quit;
    }

    void setRenderer(SDL_Renderer* ren) {
        renderer = ren;
    }

    void setFont(TTF_Font* f) {
        font = f;
    }

    void setFont1(TTF_Font* f1) {
        font1 = f1;
    }

    void setBackground(SDL_Texture* bg) {
        background = bg;
    }

    void setBackground2(SDL_Texture* bg2) {
        background2 = bg2;
    }


    void setPlayer(SDL_Texture* p) {
        player = p;
    }

    void setTarget(SDL_Texture* t) {
        target = t;
    }

    void setTarget2(SDL_Texture* t2) {
        target2 = t2;
    }

    void setTarget3(SDL_Texture* t3) {
        target3 = t3;
    }

    void setTarget4(SDL_Texture* t4) {
        target4 = t4;
    }


    int& Replace(int row, int col);
    void RenderMaze();
    void WinnerScreen();
    void Moveup();
    void Movedown();
    void Moveleft();
    void Moveright();
    void restartGame();
    void renderBullet();
    void AskForContinue();
    bool hasMovedUp() { return movedUp; }
    bool hasMovedDown() { return movedDown; }
    bool hasMovedLeft() { return movedLeft; }
    bool hasMovedRight() { return movedRight; }

};
#endif
