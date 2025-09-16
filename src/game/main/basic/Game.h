#ifndef GAME_H
#define GAME_H

#include "Configuration.h"
#include "Types.h"
#include "../class/entity/Player.h"
#include "FTXUI/component/screen_interactive.hpp"
#include <memory>
#include <optional>
#include <string>
#include <vector>

// 前向声明
class FightEvent;
class View;
class Dialog;
class Player;
class StoryController;

/**
 * @enum GameState
 * @brief 定义了游戏可能处于的几种核心状态。
 */
enum class GameState {
    MainMenu,            ///< 游戏在主菜单界面。
    InGame,              ///< 正常游戏进行中，等待玩家输入指令。
    InStory,             ///< 正在走剧情，禁止输入，禁止按侧边按钮
    AwaitingTextInput,   ///< 等待玩家进行自由文本输入。
    AwaitingChoice,      ///< 等待玩家从选项中做出选择。
};

/**
 * @struct InputRequest
 * @brief 封装了一次向玩家请求输入的完整信息。
 */
struct InputRequest {
    std::string prompt;                                             ///< 对玩家的输入提示语。
    std::vector<std::string> choices;                               ///< (用于选项) 选项的文本列表。
    std::function<void(int, const std::string&)> onChoiceSelect;    ///< (用于选项) 选中后的回调。
    std::vector<InputRule> rules;                                   ///< (用于文本) 输入规则列表。
    TextInputAction onTextSubmitDefault;                            ///< (用于文本) 无规则匹配时的默认动作。
};

/**
 * @class Game
 * @brief 游戏的核心控制类，管理游戏状态、核心服务和主循环。
 */
class Game {
public:
    Game();
    ~Game();

    void run() const;
    void startNewGame();
    void loadGame();
    void saveGame() const;
    void showGameIntro() const;
    // static void showGameSettings(); -> 逻辑在View.cpp中实现
    void exitGame() const;

    // --- 服务访问器 ---
    [[nodiscard]] Dialog& getDialog() const;
    [[nodiscard]] Player& getPlayer() const;
    [[nodiscard]] StoryController& getStoryController() const;
    [[nodiscard]] View& getView() const;
    [[nodiscard]] Configuration& getConfig() const;

    // --- 状态管理 ---
    void setGameState(GameState newState);
    [[nodiscard]] GameState getCurrentState() const;
    void clearInputRequest();
    [[nodiscard]] const std::optional<InputRequest>& getCurrentInputRequest() const;

    // --- 输入请求接口 ---
    void requestTextInput(const std::string& prompt, 
                          const std::vector<InputRule>& rules, 
                          TextInputAction onSubmitDefault);
    void requestChoice(const std::string& prompt, 
                       const std::vector<std::string>& choices, 
                       std::function<void(int, const std::string&)> onSelect);

    // --- 战斗管理 ---
    void setCurrentBattle(std::shared_ptr<FightEvent> battle);
    void clearCurrentBattle();
    bool isInBattle() const;
    std::shared_ptr<FightEvent> getCurrentBattle();
    std::shared_ptr<Player> getPlayerSharedPtr();

    bool isFirstNewGame = true; // 标记是否是第一次开始新游戏

    /**
     * @brief 检查输入请求是否已更改，并在检查后消耗此标志。
     * @return 如果自上次检查后发生过更改，则返回 true。
     */
    bool checkAndConsumeInputRequestChanged();

private:

    GameState currentState_;
    std::optional<InputRequest> inputRequest_;

    std::unique_ptr<View> view_;
    std::unique_ptr<Dialog> dialog_;
    std::unique_ptr<Player> player_;
    std::unique_ptr<StoryController> storyController_;
    
    Configuration* config_;

    // 战斗相关成员
    std::shared_ptr<FightEvent> currentBattle_;
    bool inBattle_;

    bool inputRequestChanged_ = false;
};

inline static const std::string VOICEOVER; // 旁白

#endif // GAME_H
