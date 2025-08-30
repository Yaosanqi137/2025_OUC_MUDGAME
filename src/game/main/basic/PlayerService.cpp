#include "PlayerService.h"
#include "SceneManager.h"

PlayerService& PlayerService::getInstance() {
    static PlayerService instance;
    return instance;
}

void PlayerService::registerPlayer(std::string name) {
    player.setName(name);
}

Player &PlayerService::getPlayer() {
    return player;
}

const std::string &PlayerService::getCurrentSceneID() {
    SceneManager& sceneManager = SceneManager::getInstance();
    sceneManager.getCurrentSceneID();
}

PlayerService::PlayerService() = default;

PlayerService::~PlayerService() = default;
