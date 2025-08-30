#ifndef PLAYER_SERVICE_H
#define PLAYER_SERVICE_H

#include "../class/entity/player/Player.h"

class PlayerService {
public:
    // 禁用复制和赋值
    PlayerService(const PlayerService&) = delete;
    PlayerService& operator=(const PlayerService&) = delete;

    // 单例访问
    static PlayerService& getInstance();
    
    // 玩家注册
    void registerPlayer(std::string name);

    const std::string& getCurrentSceneID(); 

    Player& getPlayer();
private:
    PlayerService();
    ~PlayerService();

    Player player;
};

#endif // PLAYER_SERVICE_H