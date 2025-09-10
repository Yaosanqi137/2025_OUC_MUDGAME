#ifndef STORYCONTROLLER_H
#define STORYCONTROLLER_H

#include "Types.h"
#include <chrono>
#include <map>
#include <string>

struct DialogNode;

/**
 * @class StoryController
 * @brief 游戏剧情的核心驱动引擎。
 * @details 负责解析对话数据、处理玩家选择，并根据剧情数据驱动游戏流程。
 */
class StoryController {
public:
    /**
     * @brief 构造函数。
     * @param game 对Game核心对象的引用。
     */
    explicit StoryController(Game& game);

    /**
     * @brief 从指定的对话节点ID开始一段剧情。
     * @param dialogNodeID 剧情起始节点的唯一ID。
     */
    void startStory(unsigned int dialogNodeID);
    
    /**
     * @brief 处理单个对话节点，通常由内部逻辑调用。
     * @param dialogNodeID 要处理的节点的唯一ID。
     */
    void processNodeByID(unsigned int dialogNodeID);

    /**
     * @brief 驱动异步剧情队列的核心更新函数。
     * @details 此函数应在游戏的主循环中（例如每帧）被调用，以处理延迟、动画等非阻塞性事件。
     */
    void update();

private:
    /**
     * @brief 真正处理DialogNode对象的内部函数。
     * @param node 指向要处理的DialogNode对象的指针。
     */
    void processNode(const DialogNode* node);

    /**
     * @brief 注册所有的对话数据到数据库中。
     * @details 通过DialogRegistry自动查找并加载所有章节的对话数据。
     */
    void registerDialogs();

    Game& game_; ///< 对Game核心对象的引用。
    std::map<unsigned int, const DialogNode*> dialogDatabase_; ///< 存储所有对话节点的数据库。

    // --- 剧情序列状态成员 ---
    StorySequence active_sequence_;                     ///< 当前正在执行的剧情序列。
    size_t sequence_step_ = 0;                          ///< 指向序列中当前步骤的索引。
    std::chrono::steady_clock::time_point waitUntil_;  ///< 如果当前是停顿步骤，记录需要等待到的时间点。
    unsigned int pending_next_node_id_ = 0;             ///< 序列结束后的跳转ID
};

#endif // STORYCONTROLLER_H