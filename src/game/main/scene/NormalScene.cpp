#include "NormalScene.h"
#include "../basic/PlayerService.h"

void NormalScene::onEnter() {

}

void NormalScene::onExit() {
    
}

void NormalScene::processInput(const std::string& input) {
    
}

void NormalScene::processImmediateKey(char key) {
    Player& player = PlayerService::getInstance().getPlayer();

    // TODO: solve input process
    system("pause>NUL");
    switch (key) {
    case 'w':
    case 'W':
        player.move(0, 1);
        break;
    case 'a':
    case 'A':
        player.move(-1, 0);
        break;
    case 's':
    case 'S':
        player.move(0, -1);
        break;
    case 'd':
    case 'D':
        player.move(1, 0);
        break;
    default:
        break;
    }
}

void NormalScene::update() {
    
}

void NormalScene::render() const {    // 获取终端实例
    Terminal& terminal = Terminal::getInstance();
    terminal.clearScreen();

    int x = 1;
    int y = 1;
    int gameWidth = 60;
    int gameHeight = 18;
    int boxHeight = 8;
    int margin = 1;

    terminal.setCursorPosition(0, 0);
    terminal.print("示例文本");

    drawTitledBorder(x, y, gameWidth, gameHeight, "Boxing Game");
    drawTitledBorder(x, y + gameHeight + margin, gameWidth, boxHeight, "History Info");

    // 绘制玩家
    Player player = PlayerService::getInstance().getPlayer();
    if (player.getX() == -1 && player.getY() == -1) {
        player.setPos(4, 4);
    }
    
    terminal.setCursorPosition(player.getX(), player.getY());
    terminal.print("@");

    terminal.setCursorPosition(x + 2, y + gameHeight + boxHeight - margin);
    terminal.print("> ");
}

// 消息系统实现
void NormalScene::addMessage(const std::string& msg) {
    messages.push_back(msg);
    if (messages.size() > maxMessages) {
        messages.pop_front();
    }
}

void NormalScene::clearMessages() {
    messages.clear();
}

const std::deque<std::string>& NormalScene::getMessages() const {
    return messages;
    
}
