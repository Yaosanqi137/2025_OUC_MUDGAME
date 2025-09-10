#include "Dialog.h"
#include "Game.h"
#include "StoryController.h"
#include "View.h"
#include "../class/entity/Player.h"
#include "../GameStory/GameProcess.h"
#include "GameTime.h"

#include <iostream>

Game::Game() : currentState_(GameState::MainMenu)  {
    view_ = std::make_unique<View>(*this);
    dialog_ = std::make_unique<Dialog>(*this);
    player_ = std::make_unique<Player>(*this);
    storyController_ = std::make_unique<StoryController>(*this);

    GameTime(); // 初始化游戏时间
}

Game::~Game() {
    // TODO: 游戏退出析构函数
}

void Game::run() {
    if (view_) {
        view_->showMainMenu();
    }
}

void Game::startNewGame() {
    if (view_) {
        View::showLoadingScreen("正在加载新游戏");
    }
    std::cout << "开始新游戏..." << std::endl;
    // TODO
    // ... 此处是开始新游戏的具体逻辑 ...
    GameProcess::newStart(*this);
    if (view_) {
        view_->showGameScreen();
    }
}

void Game::loadGame() const {
    std::string saveName = "default";
    if (view_) {
        View::showLoadingScreen("正在打开存档{" + saveName + "}");
    }
    std::cout << "读取存档..." << std::endl;
    // TODO
    // ... 此处是读取存档的具体逻辑 ...
}

void Game::showGameIntro() const {
    std::cout << "游戏介绍..." << std::endl;
    if (view_) {
        View::showGameIntroScreen();
    }
}

void Game::showGameSettings() {
    std::cout << "游戏设置..." << std::endl;
    // TODO
    // ... 显示设置的具体逻辑 ...
}

void Game::exitGame() {
    std::cout << "退出游戏..." << std::endl;
    requestExit();
}

// --- 服务访问器实现 ---
Dialog& Game::getDialog() const {
    return *dialog_;
}

Player& Game::getPlayer() const {
    return *player_;
}

StoryController& Game::getStoryController() const {
    return *storyController_;
}

View& Game::getView() const {
    return *view_;
}

// --- 状态管理实现 ---
void Game::setGameState(GameState newState) {
    currentState_ = newState;
}

GameState Game::getCurrentState() const {
    return currentState_;
}

const std::optional<InputRequest>& Game::getCurrentInputRequest() const {
    return inputRequest_;
}

void Game::clearInputRequest() {
    inputRequest_.reset();
    setGameState(GameState::InGame);
}


// --- 输入请求接口实现 ---
void Game::requestTextInput(const std::string& prompt, 
                            const std::vector<InputRule>& rules, 
                            TextInputAction onSubmitDefault) {
    inputRequest_ = InputRequest{
        .prompt = prompt,
        .rules = rules,
        .onTextSubmitDefault = std::move(onSubmitDefault)
    };
    setGameState(GameState::AwaitingTextInput);
}

void Game::requestChoice(const std::string& prompt, 
                         const std::vector<std::string>& choices, 
                         std::function<void(int, const std::string&)> onSelect) {
    inputRequest_ = InputRequest{
        .prompt = prompt,
        .choices = choices,
        .onChoiceSelect = std::move(onSelect)
    };
    setGameState(GameState::AwaitingChoice);
}

void Game::setScreen(ftxui::ScreenInteractive* screen) {
    screen_ = screen;
}

void Game::requestExit() const {
    if (screen_) {
        // 发布一个自定义的特殊事件。
        // UI线程的事件循环会在下一轮处理这个事件。
        screen_->Post(ftxui::Event::Special("GAME_EXIT_REQUEST"));
    }
}
