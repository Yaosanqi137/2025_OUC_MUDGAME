#ifndef GAME_H
#define GAME_H

#include "PlayerService.h"
#include "SceneManager.h"
#include "Terminal.h"
#include "../class/entity/player/Player.h"

/**
 * @class Game
 * @brief 游戏主控制类，管理游戏生命周期和主循环
 */
class Game {
public:
    Game();
    ~Game();
    
    void run();

private:
    void init();
    void processInput();
    void initScenes();
    void shutdown();

    SceneManager& sceneManager = SceneManager::getInstance();
    Terminal& terminal = Terminal::getInstance();
    PlayerService& playerService = PlayerService::getInstance();
    bool running = false;
};

#endif // GAME_H