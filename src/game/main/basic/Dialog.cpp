#include "Dialog.h"
#include "Game.h"
#include "GameTime.h"
#include "InputProcess.h"
#include "StoryController.h"
#include "../event/TrainingEvent.h"
#include "../event/FightEvent.h"
#include "BattleCommandHandler.h" // 包含新的命令处理器
#include "../GameStory/Scene/ShopScene.h"
#include "../GameStory/Scene/PharmacyScene.h"
#include "../GameStory/Scene/CafeScene.h"
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

    //检查是否在战斗中
    if (game_logic_.isInBattle()) {
        // 直接处理战斗命令
        auto battle = game_logic_.getCurrentBattle();
        if (!battle) {
            addMessage("<SYSTEM>", "错误：战斗状态异常");
            game_logic_.clearCurrentBattle();
            return;
        }

        // 检查是否是玩家回合
        if (!battle->isPlayerTurn()) {
            addMessage("<SYSTEM>", "现在不是你的回合，请等待敌人行动");
            return;
        }

        // 处理战斗命令
        if (input.find("/enemy attack pass") != std::string::npos) {
            // 处理跳过回合
            battle->playerChooseAction(1, 0); // 1 表示跳过回合
            return;
        } else if (input.rfind("/enemy attack skill ", 0) == 0) {
            // 处理技能攻击
            std::string idStr = input.substr(20);
            try {
                int skillId = std::stoi(idStr);
                auto& skills = game_logic_.getPlayer().getSkills();
                int index = -1;
                
                for (int i = 0; i < skills.size(); i++) {
                    if (skills[i]->getId() == skillId && skills[i]->isAttackSkill()) {
                        index = i;
                        break;
                    }
                }
                
                if (index != -1) {
                    battle->playerChooseAction(0, index); // 0 表示技能攻击
                } else {
                    addMessage("<SYSTEM>", "无效的技能ID或这不是攻击技能");
                }
            } catch (const std::exception& e) {
                addMessage("<SYSTEM>", "错误: 无效的技能ID格式");
            }
            return;
        } else if (input.find("/enemy kill") != std::string::npos){
            // 设置敌人血量为1点 - 调试命令
            battle->setEnemyHealthToLow();
            addMessage("<SYSTEM>", "调试命令: 已将敌人血量设置为1点");
            return;
        } else {
            // 不是战斗命令，显示错误消息
            addMessage("<SYSTEM>", "在战斗中，请使用战斗命令: /enemy attack skill <id> 或 /enemy attack pass");
            return;
        }
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
            game_logic_.getDialog().addMessage("<SYSTEM>", "/enemy: 查看敌人相关操作");
            // game_logic_.getDialog().addMessage("<SYSTEM>", "/npc: 查看附加能够对话的NPC");
            // game_logic_.getDialog().addMessage("<SYSTEM>", "/chat: 与NPC对话，格式 /chat NPC名称"); 有时间再说
        } else if (input == "/clear") {
            game_logic_.getDialog().clearHistory(); // 清除历史记录
        } else if (input == "/use") {
            if (loc == "家") {
                game_logic_.getDialog().addMessage("<SYSTEM>", "/sleep: 睡觉，用于恢复疲劳并跳过一天");
                game_logic_.getDialog().addMessage("<SYSTEM>", "恢复10点生命值和50点体力");
            }
            if (loc == "工地") {
                game_logic_.getDialog().addMessage("<SYSTEM>", "/work: 工作，赚取微薄的收入");
                game_logic_.getDialog().addMessage("<SYSTEM>", "每次工作赚取40-80元，消耗15点体力,15点饥饿值");
                game_logic_.getDialog().addMessage("<SYSTEM>", "预计花费3-5小时");
            }
            if (loc == "商店" || loc == "药店" || loc == "咖啡馆") {
                game_logic_.getDialog().addMessage("<SYSTEM>", "/buy: 打开购买界面，购买食物、饮料、药品等");
                game_logic_.getDialog().addMessage("<SYSTEM>", "购买的物品会直接放入背包");
                game_logic_.getDialog().addMessage("<SYSTEM>", "但是买东西千万不要忘记带钱，否则...");
            }
        } else if (input == "/sleep") {
            if (loc == "家") {
                game_logic_.getDialog().addMessage("", "你躺在了你的床上，闭上眼睛，渐渐进入了梦乡...");
                GameTime::addDay(1);
                static std::mt19937 rng(std::random_device{}());
                std::uniform_int_distribution<int> distHour(-8, -4);
                std::uniform_int_distribution<int> distMinute(-30, 30);
                std::uniform_int_distribution<int> distSleep(3, 5);
                GameTime::addHour(distHour(rng));
                GameTime::addMinute(distMinute(rng));
                for (int i = 0; i < distSleep(rng); i++) {
                    game_logic_.getDialog().addMessage("<PLAYER_NAME>", "Zzz...");
                }
                game_logic_.getDialog().addMessage("", "你醒来了，感觉精神焕发！");
                game_logic_.getPlayer().addHealth(5);
                game_logic_.getPlayer().addFatigue(50);
            } else {
                game_logic_.getDialog().addMessage("<SYSTEM>", "你只能在家里睡觉！");
            }
        } else if (input == "/work") {

            if (loc == "工地") {
                if (game_logic_.getPlayer().getFatigue() < 15) {
                    game_logic_.getDialog().addMessage("<SYSTEM>", "你太累了，无法工作！");
                    return;
                } else if (game_logic_.getPlayer().getHunger() < 15) {
                    game_logic_.getDialog().addMessage("<SYSTEM>", "你太饿了，无法工作！");
                    return;
                }
                static std::mt19937 rng(std::random_device{}());
                std::uniform_int_distribution<int> distMoney(40, 80);
                std::uniform_int_distribution<int> distHour(3, 5);
                int earned = distMoney(rng);
                game_logic_.getPlayer().addSavings(earned);
                game_logic_.getPlayer().addFatigue(-15);
                game_logic_.getPlayer().addHunger(-15);
                GameTime::addHour(distHour(rng));
                game_logic_.getDialog().addMessage("<SYSTEM>", "你在工地辛勤工作，赚取了 " + std::to_string(earned) + " 元");
                game_logic_.getDialog().addMessage("<SYSTEM>", "但你也消耗了15点体力和15点饥饿值");
                game_logic_.getDialog().addMessage("<SYSTEM>", "你觉得你需要休息一下，吃点东西");
            } else {
                game_logic_.getDialog().addMessage("<SYSTEM>", "你只能在工地工作！");
            }
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
            if (loc == "拳击馆") {
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
            } else {
                game_logic_.getDialog().addMessage("<SYSTEM>", "你只能在拳击馆查看训练状态！");
                return;
            }
        } else if (input == "/train strength") {
            if (loc != "拳击馆") {
                game_logic_.getDialog().addMessage("<SYSTEM>", "你只能在拳击馆训练！");
                return;
            }
            game_logic_.getPlayer().getTrainingSystem()->train(TrainingType::STRENGTH, game_logic_);
            
        } else if (input == "/train agility") {
            if (loc != "拳击馆") {
                game_logic_.getDialog().addMessage("<SYSTEM>", "你只能在拳击馆训练！");
                return;
            }
            game_logic_.getPlayer().getTrainingSystem()->train(TrainingType::AGILITY, game_logic_);
            
        } else if (input == "/train stamina") {
            if (loc != "拳击馆") {
                game_logic_.getDialog().addMessage("<SYSTEM>", "你只能在拳击馆训练！");
                return;
            }
            game_logic_.getPlayer().getTrainingSystem()->train(TrainingType::STAMINA, game_logic_);
        } else if (input == "/enemy") {
            game_logic_.getDialog().addMessage("<SYSTEM>", "敌人相关命令:");
            game_logic_.getDialog().addMessage("<SYSTEM>", "/enemy show: 显示下一个敌人的信息");
            game_logic_.getDialog().addMessage("<SYSTEM>", "/enemy battle: 开始与下一个敌人的战斗");
            game_logic_.getDialog().addMessage("<SYSTEM>", "在战斗中可使用:");
            game_logic_.getDialog().addMessage("<SYSTEM>", "/enemy attack skill <id>: 使用技能攻击");
            game_logic_.getDialog().addMessage("<SYSTEM>", "/enemy attack pass: 跳过回合");
        }else if (input == "/enemy show") {
            if (loc != "比赛场地") {
                game_logic_.getDialog().addMessage("<SYSTEM>", "你只能在比赛场地查看敌人！");
                return;
            }
            int nextEnemyId = game_logic_.getPlayer().getHighestUnlockedEnemy() + 1;
            BattleCommandHandler::showEnemyInfo(game_logic_, nextEnemyId);
        } else if (input == "/enemy battle") {
            if (loc != "比赛场地") {
                game_logic_.getDialog().addMessage("<SYSTEM>", "你只能在比赛场地进行拳击比赛！");
                return;
            }
            int EnemyId = game_logic_.getPlayer().getHighestUnlockedEnemy();
            BattleCommandHandler::startBattle(game_logic_, EnemyId);
        } else {
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
