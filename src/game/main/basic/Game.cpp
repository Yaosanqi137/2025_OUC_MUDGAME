#include "Game.h"
#include "Dialog.h"
#include "StoryController.h"
#include "View.h"
#include "SaveClass.h"
#include "../class/entity/Player.h"
#include "../GameStory/GameProcess.h"
#include "../event/FightEvent.h"

#include <iostream>
#include <cstdlib>

Game::Game() : currentState_(GameState::MainMenu), inBattle_(false) {
    isFirstNewGame = !SaveClass::getInstance()->doesSaveExist();

    config_ = Configuration::getInstance();

    dialog_ = std::make_unique<Dialog>(*this);
    player_ = std::make_unique<Player>(*this);
    storyController_ = std::make_unique<StoryController>(*this);

    view_ = std::make_unique<View>(*this);
    
    // 初始化selfWeakPtr_
    auto selfPtr = std::shared_ptr<Game>(this, [](Game*){});
}

Game::~Game() {
    // TODO: 游戏退出析构函数
}

void Game::run() const {
    if (view_) {
        view_->showMainMenu();
    }
}

void Game::startNewGame() {
    if (isFirstNewGame) {
        isFirstNewGame = false;
        if (view_) {
            View::showLoadingScreen("正在加载新游戏");
        }
        std::cout << "开始新游戏..." << std::endl;
        // TODO
        // ... 此处是开始新游戏的具体逻辑 ...
        GameProcess::newStart(*this);
    }
    if (view_) {
        view_->showGameScreen();
    }
}

void Game::loadGame() {
    if (!SaveClass::getInstance()->doesSaveExist()) {
        View::showTemporaryPopup("没有找到存档文件，请先开始新游戏。");
        return;
    }

    // 步骤 2: 存档文件存在，继续正常的加载流程
    View::showLoadingScreen("正在打开存档...");

    if (SaveClass::getInstance()->loadGame(*this)) {
        // 加载成功后:

        // 1. 清除任何可能残留的输入请求。
        //    即使我们保存时降级了状态，这样做也是一个好的保险措施。
        this->clearInputRequest();

        // 2. 清理对话历史并显示成功消息。
        this->getDialog().clearHistory();
        this->getDialog().addMessage("<SYSTEM>", "存档已成功加载。");

        // 如果加载的状态是 InStory，可能需要一条额外的提示
        if (this->getCurrentState() == GameState::InStory) {
            this->getDialog().addMessage("<SYSTEM>", "正在继续之前的剧情...");
        }

        // 3. 进入游戏界面。
        //    UI会根据 SaveClass 恢复的 GameState 来正确渲染。
        view_->showGameScreen();

    } else {
        View::showTemporaryPopup("加载存档失败，文件可能已损坏。", 4);
        isFirstNewGame = true;
    }
}

void Game::saveGame() const {
    SaveClass::getInstance()->saveGame(const_cast<Game&>(*this));
}

void Game::showGameIntro() const {
    std::cout << "游戏介绍..." << std::endl;
    if (view_) {
        View::showGameIntroScreen();
    }
}

void Game::exitGame() const {
    std::cout << "退出游戏..." << std::endl;
    std::exit(0);
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

Configuration& Game::getConfig() const {
    return *config_;
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

// --- 战斗管理实现 ---
void Game::setCurrentBattle(std::shared_ptr<FightEvent> battle) {
    currentBattle_ = battle;
    inBattle_ = (battle != nullptr);
}

void Game::clearCurrentBattle() {
    currentBattle_ = nullptr;
    inBattle_ = false;
    
    // 确保游戏状态回到正常状态
    if (currentState_ == GameState::InGame) {
        // 如果已经是游戏状态，不需要改变
    } else {
        setGameState(GameState::InGame);
    }
    
    // 清除输入请求
    clearInputRequest();
}

bool Game::isInBattle() const {
    return inBattle_;
}

std::shared_ptr<FightEvent> Game::getCurrentBattle() {
    return currentBattle_;
}

std::shared_ptr<Player> Game::getPlayerSharedPtr() {
    // 直接返回 Player 的共享指针，使用空删除器
    return std::shared_ptr<Player>(&getPlayer(), [](Player*){});
}
