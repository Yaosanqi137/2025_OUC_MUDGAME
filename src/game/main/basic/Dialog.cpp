#include "Dialog.h"
#include "Game.h"
#include "GameTime.h"
#include "InputProcess.h"
#include "StoryController.h"
#include "../event/TrainingEvent.h"
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
            game_logic_.getDialog().addMessage("<SYSTEM>", "/train: 训练及帮助菜单");
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
            game_logic_.getDialog().addMessage("<SYSTEM>", "当前技能点：" + std::to_string((int)game_logic_.getPlayer().getSkillPoints()));
            game_logic_.getDialog().addMessage("<SYSTEM>", "技能命令用法:");
            game_logic_.getDialog().addMessage("<SYSTEM>", "/skill show all      - 显示所有技能");
            game_logic_.getDialog().addMessage("<SYSTEM>", "/skill show canlearn - 显示可学习技能");
            game_logic_.getDialog().addMessage("<SYSTEM>", "/skill learn <id>    - 学习指定ID的技能");
        }else if (input == "/skill show all") {
            // 显示所有技能
            auto allSkills = game_logic_.getPlayer().getAllSkillsInfo();
            game_logic_.getDialog().addMessage("<SYSTEM>", "=== 所有技能列表 ===");
            for (const auto& skillInfo : allSkills) {
                game_logic_.getDialog().addMessage("<SYSTEM>", skillInfo);
            }
        
        } else if (input == "/skill show canlearn") {
            // 显示可学习技能
            auto learnableSkills = game_logic_.getPlayer().getLearnableSkillsInfo();
            game_logic_.getDialog().addMessage("<SYSTEM>", "=== 可学习技能 ===");
            if (learnableSkills.empty()) {
                game_logic_.getDialog().addMessage("<SYSTEM>", "暂无可以学习的技能");
            } else {
                    for (const auto& skillInfo : learnableSkills) {
                        game_logic_.getDialog().addMessage("<SYSTEM>", skillInfo);
                    }
                }
            
        } else if (input.rfind("/skill learn ", 0) == 0) {
            // 学习技能
            std::string idStr = input.substr(12); // 去掉 "/skill learn "
            try {
                int skillId = std::stoi(idStr);
                bool success = game_logic_.getPlayer().learnSkillById(skillId);
                if (success) {
                    game_logic_.getDialog().addMessage("<SYSTEM>", "技能学习成功！");
                    game_logic_.getDialog().addMessage("<SYSTEM>", "剩余技能点: " + 
                                                    std::to_string((int)game_logic_.getPlayer().getSkillPoints()));
                } else {
                    game_logic_.getDialog().addMessage("<SYSTEM>", "技能学习失败！");
                    game_logic_.getDialog().addMessage("<SYSTEM>", "可能的原因: 技能点不足、前置条件未满足或技能ID无效");
                }
            } catch (const std::exception& e) {
                game_logic_.getDialog().addMessage("<SYSTEM>", "错误: 无效的技能ID格式");
            }
        } else if (input == "/train") {
            // 训练帮助菜单
            auto helpInfo = TrainingEvent::getTrainingHelp();
            // 按行分割帮助信息并逐行显示
            size_t pos = 0;
            while ((pos = helpInfo.find('\n')) != std::string::npos) {
                game_logic_.getDialog().addMessage("<SYSTEM>", helpInfo.substr(0, pos));
                helpInfo.erase(0, pos + 1);
            }
            if (!helpInfo.empty()) {
                game_logic_.getDialog().addMessage("<SYSTEM>", helpInfo);
            }
            
        } else if (input == "/train status") {
            // 查看训练状态
            auto statusInfo = game_logic_.getPlayer().getTrainingSystem()->getTrainingStatus();
            // 按行分割状态信息并逐行显示
            size_t pos = 0;
            while ((pos = statusInfo.find('\n')) != std::string::npos) {
                game_logic_.getDialog().addMessage("<SYSTEM>", statusInfo.substr(0, pos));
                statusInfo.erase(0, pos + 1);
            }
            if (!statusInfo.empty()) {
                game_logic_.getDialog().addMessage("<SYSTEM>", statusInfo);
            }
            
        } else if (input == "/train strength") {
            game_logic_.getPlayer().getTrainingSystem()->train(TrainingType::STRENGTH, game_logic_);
            
        } else if (input == "/train agility") {
            game_logic_.getPlayer().getTrainingSystem()->train(TrainingType::AGILITY, game_logic_);
            
        } else if (input == "/train stamina") {
            game_logic_.getPlayer().getTrainingSystem()->train(TrainingType::STAMINA, game_logic_);
        }else {
            // 对话，将其添加到历史记录中
            addMessage(game_logic_.getPlayer().getName(), input);
        }
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
