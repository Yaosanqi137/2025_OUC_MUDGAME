#ifndef GAMEPROCESS_H
#define GAMEPROCESS_H

#include "../basic/Game.h"
#include "../basic/InputProcess.h"
#include "Story/chapter1.h"

/**
 * @class GameProcess
 * @brief 处理特定游戏流程的静态工具类，如游戏开场。
 */
class GameProcess {
public:
    /**
     * @brief 处理新游戏的开场流程。
     * @param game_logic_ 对Game核心对象的引用。
     */
    static void newStart(Game& game_logic_) {
        // 显示第一句引导语
        game_logic_.getStoryController().processNodeByID(1);
        
        // 请求玩家输入名字 also a example of requestTextInput()
        game_logic_.requestTextInput(
            // 参数1: prompt - 输入提示
            "输入你的名字:",
            // 参数2: rules - 一个规则列表，用于处理特殊输入
            {
                // 可以留空，或者添加规则，例如：
                {
                    // condition: 输入 "yaosanqi"
                    .condition = [](std::string_view input){ return trim(std::string(input)) == "yaosanqi"; },
                    // action: 给予特殊回应
                    .action = [](Game& g, std::string_view input){
                        g.getDialog().addMessage("系统", "你发现了一个彩蛋:)！");
                        std::string name = trim(std::string(input));
                        g.getPlayer().setName(name);
                        
                        // 名字输入成功后，继续剧情
                        g.getStoryController().startStory(2);
                    }
                }
            },
            // 参数3: on_submit_default - 默认动作 (当所有规则都不匹配时执行)
            [](Game& game_logic_, std::string_view input) {
                std::string name = trim(std::string(input));
                // 进行名字有效性验证
                if (name.empty() || getLength(name) > 10 || getLength(name) < 2) {
                    game_logic_.getDialog().addMessage("<UNKNOWN>", "名字不合适，请重新输入一个2-10个字的名字。");
                    newStart(game_logic_);
                    return;
                }
                
                game_logic_.getPlayer().setName(name);
                
                // 名字输入成功后，继续剧情
                game_logic_.getStoryController().startStory(2);
            }
        );
        
        // 步骤 4: 启动游戏主界面和循环
        game_logic_.getView().showGameScreen();
    }
};

#endif // GAMEPROCESS_H