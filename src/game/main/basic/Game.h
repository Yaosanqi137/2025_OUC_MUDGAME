// Game.h
#pragma once

#include "PlayerService.h"
#include <memory>

class View;

class Game {
public:
    Game();
    ~Game();

    void run();
    void startNewGame();
    void loadGame();
    void showGameIntro();
    void showGameSettings();
    void exitGame();

private:
    bool running = false;
    PlayerService& playerService = PlayerService::getInstance();

    std::unique_ptr<View> view_; 
};