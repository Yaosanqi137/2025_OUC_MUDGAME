// Game.h
#pragma once

#include "PlayerService.h"
#include <memory>

class View;

class Game {
public:
    Game();
    ~Game();

    void yao37();

    void run();
    void startNewGame();
    void loadGame();
    void showGameIntro();
    void showGameSettings();
    void exitGame();

private:
    void init();

    bool running = false;
    PlayerService& playerService = PlayerService::getInstance();

    std::unique_ptr<View> view_; 
};