#include "Dialog.h"
#include "Game.h"
#include "InputProcess.h"
#include "StoryController.h"

Dialog::Dialog(Game& game_logic) : game_logic_(game_logic) {}

void Dialog::addMessage(const std::string& who, const std::string& content) {

    // --- 占位符替换逻辑 ---
    std::vector<std::string> processed = { who, content} ;
    for (auto& str : processed) {
        // <PLAYER_NAME> 占位符
        std::string placeholder = "<PLAYER_NAME>";
        size_t pos = str.find(placeholder);
        if (pos != std::string::npos) {
            // 如果找到，用当前的玩家名替换它
            str.replace(pos, placeholder.length(), game_logic_.getPlayer().getName());
        }

        // <UNKNOWN> 占位符
        placeholder = "<UNKNOWN>";
        pos = str.find(placeholder);
        if (pos != std::string::npos) {
            str.replace(pos, placeholder.length(), "???");
        }

        // <SYSTEM> 占位符
        placeholder = "<SYSTEM>";
        pos = str.find(placeholder);
        if (pos != std::string::npos) {
            str.replace(pos, placeholder.length(), "系统");
        }
    }

    history_.push_back({processed.at(0), processed.at(1), std::chrono::steady_clock::now()});

    // 如果历史记录过长，则移除最旧的一条
    if (history_.size() > MAX_HISTORY_SIZE) {
        history_.erase(history_.begin());
    }
}

void Dialog::processPlayerInput(std::string& input) {
    if (input.empty()) {
        return;
    }

    // 命令与对话
    if (input[0] == '/') {
        // 命令，交给 Game 逻辑层处理
        input = trim(input);

        // TODO: 在这里解析并执行具体的命令
        if (input == "/help") {
            game_logic_.getDialog().addMessage("系统", "可用命令: /status, /save, /quit");
        } else if (input == "/status") {
            game_logic_.getDialog().addMessage("系统", "你感觉状态良好。"); // TODO: 需要API接入
        } else {
            game_logic_.getDialog().addMessage("系统", "未知命令: " + input);
        }
    } else {
        // 对话，将其添加到历史记录中
        addMessage(game_logic_.getPlayer().getName(), input);
    }
}

const std::vector<DialogMessage>& Dialog::getHistory() const {
    return history_;
}

void Dialog::clearHistory() {
    history_.clear();
    history_was_cleared_ = true; // to be checked in GameLayout.cpp
}

bool Dialog::historyWasClearedAndConsume() const {
    if (history_was_cleared_) {
        history_was_cleared_ = false;
        return true;
    }
    return false;
}
