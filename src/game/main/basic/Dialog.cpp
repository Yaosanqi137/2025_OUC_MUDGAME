#include "Dialog.h"
#include "Game.h"
#include "GameTime.h"
#include "InputProcess.h"
#include "StoryController.h"
#include <random>

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
        auto loc = game_logic_.getPlayer().getLocation();

        // TODO: 在这里解析并执行具体的命令
        if (input == "/help") {
            game_logic_.getDialog().addMessage("<SYSTEM>", "--==指令帮助菜单==--");
            game_logic_.getDialog().addMessage("<SYSTEM>", "可用命令:");
            game_logic_.getDialog().addMessage("<SYSTEM>", "/help: 查看本条指令帮助");
            game_logic_.getDialog().addMessage("<SYSTEM>", "/clear: 清除对话");
            game_logic_.getDialog().addMessage("<SYSTEM>", "/use: 查看本场景下能够使用的指令");
            game_logic_.getDialog().addMessage("<SYSTEM>", "/skill: 查看技能点及帮助菜单");
            // game_logic_.getDialog().addMessage("<SYSTEM>", "/npc: 查看附加能够对话的NPC");
            // game_logic_.getDialog().addMessage("<SYSTEM>", "/chat: 与NPC对话，格式 /chat NPC名称"); 有时间再说
        } else if (input == "/clear") {
            game_logic_.getDialog().clearHistory(); // 清除历史记录
        } else if (input == "/use") {
            if (loc == "家") {
                game_logic_.getDialog().addMessage("<SYSTEM>", "/sleep: 睡觉，用于恢复疲劳或跳过一天");
                game_logic_.getDialog().addMessage("<SYSTEM>", "恢复5点生命值和8点体力");
            }
            if (loc == "工地") {
                game_logic_.getDialog().addMessage("<SYSTEM>", "/work: 工作，赚取微薄的收入");
                game_logic_.getDialog().addMessage("<SYSTEM>", "每次工作赚取50-150元，消耗10点体力,10点");
            }
        } else if (input == "/sleep") {
            if (loc == "家") {
                game_logic_.getDialog().addMessage("", "你躺在了你的床上，闭上眼睛，渐渐进入了梦乡...");
                GameTime::addDay(1);
                static std::mt19937 rng(std::random_device{}());
                std::uniform_int_distribution<int> distHour(-2, 2);
                std::uniform_int_distribution<int> distMinute(-15, 15);
                std::uniform_int_distribution<int> distSleep(3, 5);
                GameTime::addHour(distHour(rng));
                GameTime::addMinute(distMinute(rng));
                for (int i = 0; i < distSleep(rng); i++) {
                    game_logic_.getDialog().addMessage("<PLAYER_NAME>", "Zzz...");
                }
                game_logic_.getDialog().addMessage("", "你醒来了，感觉精神焕发！");
                game_logic_.getPlayer().addHealth(5);
                game_logic_.getPlayer().addStamina(8);
            } else {
                game_logic_.getDialog().addMessage("<SYSTEM>", "你只能在家里睡觉！");
            }
        } else if (input == "/use") {

        }else if (input == "/skill") {
            game_logic_.getDialog().addMessage("<SYSTEM>", "当前技能点：");
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
