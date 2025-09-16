#ifndef DIALOG_H
#define DIALOG_H

#include <chrono>
#include <string>
#include <vector>

class Game;

// 用于存储单条对话消息的结构体
struct DialogMessage {
    std::string who;
    std::string content; // 完整内容
    std::chrono::steady_clock::time_point start_time; // 开始显示时间
};

class Dialog {
public:
    // 构造函数需要 Game 的引用来执行命令
    explicit Dialog(Game& game_logic);

    // 添加一条消息到对话历史（来自NPC、系统等）
    void addMessage(const std::string& who, const std::string& content);

    // 处理玩家在输入框中按下回车后的字符串
    void processPlayerInput(std::string& input);

    // 获取对话历史记录，用于渲染
    [[nodiscard]] const std::vector<DialogMessage>& getHistory() const;

    // 清除对话历史记录
    void clearHistory();

    /**
     * @brief 检查历史记录自上次检查后是否被清除过。
     * @details 这是一个"读取后即消耗"的标志。调用此函数会返回当前状态并立即重置标志为false。
     * @return 如果历史记录被清除了，则返回true。
     */
    [[nodiscard]] bool historyWasClearedAndConsume() const;

private:
    Game& game_logic_;
    std::vector<DialogMessage> history_;
    mutable bool historyWasCleared_ = false;
    static constexpr size_t MAX_HISTORY_SIZE = 32; // 限制历史记录最大长度
};

#endif // DIALOG_H
