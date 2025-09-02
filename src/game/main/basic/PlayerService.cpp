#include "PlayerService.h"

PlayerService& PlayerService::getInstance() {
    static PlayerService instance;
    return instance;
}

void PlayerService::registerPlayer(const std::string& name) {
    player.setName(name);
}

Player &PlayerService::getPlayer() {
    return player;
}

PlayerService::PlayerService() = default;

PlayerService::~PlayerService() = default;
