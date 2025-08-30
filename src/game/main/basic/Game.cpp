#include "Game.h"
#include "PlayerService.h"
#include "SceneManager.h"
#include "Terminal.h"
#include "../scene/NormalScene.h"
#include "../class/entity/player/Player.h"
#include <iostream>
#include <memory>

Game::Game() {
    init();
}

Game::~Game() {
    shutdown();
}

void Game::run() {
    running = true;

    // 游戏主循环
    while (running) {
        // 获取当前场景
        auto* scene = sceneManager.getCurrentScene();

        // 渲染当前场景
        if (scene) scene->render();

        // 处理输入
        processInput();
    }
}

void Game::init() {
    // 初始化终端
    Terminal::init();

    // 初始化游戏场景
    initScenes();
}

void Game::initScenes() {    
    sceneManager.registerScene("start", std::make_unique<NormalScene>());
    sceneManager.switchScene("start");
}

void Game::processInput() {
    auto* scene = sceneManager.getCurrentScene();
    if (!scene) return;

    // 处理即时按键
    // TODO: 目前不知为何不工作
    char immediateKey = terminal.getKey();
    if (immediateKey != ':') {
        scene->processImmediateKey(immediateKey);
    }

    // // 处理完整命令
    // std::string input = terminal.getLine();
    // if (!input.empty()) {
    //     if (input == "quit") {
    //         running = false;
    //     } else {
    //         scene->processInput(input);
    //     }
    // }
}

void Game::shutdown() {
}
