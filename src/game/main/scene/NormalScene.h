#ifndef NORMAL_SCENE_H
#define NORMAL_SCENE_H

#include "../basic/SceneBase.h"
#include "../class/entity/player/Player.h"
#include <deque>
#include <memory>

class NormalScene : public SceneBase {
public:
    void onEnter() override;
    void onExit() override;
    void processInput(const std::string& input) override;
    void processImmediateKey(char key) override;
    void update() override;
    void render() const override;

    // 消息系统
    void addMessage(const std::string& msg);
    void clearMessages();
    const std::deque<std::string>& getMessages() const;

private:
    // 消息存储
    std::deque<std::string> messages;
    const size_t maxMessages = 10;

    // 玩家
    std::shared_ptr<Player> player;
};

#endif // NORMAL_SCENE_H