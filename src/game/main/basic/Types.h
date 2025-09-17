#ifndef GAME_TYPES_H
#define GAME_TYPES_H

#include <chrono>
#include <functional>
#include <string>
#include <string_view>
#include <variant>

class Game;

/**
 * @brief 用于选项(Choice)或通用函数回调的动作类型。
 * @details 这种动作是无上下文的，仅操作Game对象。
 * @param game 对Game核心对象的引用。
 */
using ChoiceAction = std::function<void(Game&)>;

/**
 * @brief 用于文本输入(TextInput)相关动作的类型。
 * @details 这种动作会接收玩家输入的文本作为额外参数。
 * @param game 对Game核心对象的引用。
 * @param input 玩家输入的文本。
 */
using TextInputAction = std::function<void(Game&, std::string_view)>;

/**
 * @brief 输入规则的条件判断函数类型。
 * @param input 待检查的输入文本。
 * @return 如果输入满足条件，则返回true。
 */
using RuleCondition = std::function<bool(std::string_view)>;

/**
 * @brief 与输入规则匹配时执行的动作类型。
 * @param game 对Game核心对象的引用。
 * @param input 匹配规则的输入文本。
 */
using RuleAction = std::function<void(Game&, std::string_view)>;

/**
 * @struct InputRule
 * @brief 定义一条文本输入的处理规则。
 */
struct InputRule {
    RuleCondition condition; ///< 规则的匹配条件。
    RuleAction action;       ///< 条件满足时执行的动作。
};

// --- 新增：剧情序列步骤类型 ---

/**
 * @struct SpeakAction
 * @brief 代表剧情序列中的一个“说话”步骤。
 */
struct SpeakAction {
    std::string who;      ///< 说话者的名字。
    std::string content;  ///< 对话的内容。
};

/**
 * @struct WaitAction
 * @brief 代表剧情序列中的一个“停顿”步骤。
 */
struct WaitAction {
    std::chrono::milliseconds duration; ///< 需要停顿的时长。
};

/**
 * @struct ExecuteAction
 * @brief 代表剧情序列中的一个“执行自定义函数”的步骤。
 */
struct ExecuteAction {
    ChoiceAction function; ///< 要执行的函数，复用ChoiceAction类型。
};

/**
 * @typedef StoryStep
 * @brief 定义一个剧情步骤。它可以是说话、停顿或执行函数中的任意一种。
 * @details 使用std::variant来实现类型的多态。
 */
using StoryStep = std::variant<SpeakAction, WaitAction, ExecuteAction>;

/**
 * @typedef StorySequence
 * @brief 定义一个剧情序列，它是由一连串的剧情步骤组成的向量。
 */
using StorySequence = std::vector<StoryStep>;

#endif // GAME_TYPES_H