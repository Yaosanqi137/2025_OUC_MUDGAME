#ifndef SCENE_BASE_H
#define SCENE_BASE_H

#include "Terminal.h"
#include <string>
#include <functional>
#include <vector>

class SceneBase {
public:
    virtual ~SceneBase() = default;

    // 主接口
    virtual void onEnter() = 0;
    virtual void onExit() = 0;
    virtual void processInput(const std::string& input) = 0;
    virtual void processImmediateKey(char key) = 0;
    virtual void update() = 0;
    virtual void render() const = 0;

    // 边框绘制功能
    void drawBorder(int x, int y, int width, int height) const;
    void drawTitledBorder(int x, int y, int width, int height,
                          const std::string& title) const;

protected:
    Terminal& terminal = Terminal::getInstance();

private:
    // 获取边框字符
    void getBorderChars(std::string& topLeft, std::string& topRight,
                        std::string& bottomLeft, std::string& bottomRight,
                        std::string& horizontal, std::string& vertical) const;
};

#endif // SCENE_BASE_H