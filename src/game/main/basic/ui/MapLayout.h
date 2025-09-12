#ifndef MAPLAYOUT_H
#define MAPLAYOUT_H

#include "FTXUI/component/component_base.hpp"
#include "FTXUI/component/component.hpp"
#include <string>
#include <vector>
#include <map>

class Game; // 前向声明

// 定义地图上一个地点的数据结构
struct MapLocation {
    std::string id;          // 唯一标识符，如 "home"
    std::string name;        // 显示名称，如 "家"
    int x, y;                // 在画布上的坐标

    // 用于键盘导航的邻居ID
    std::string nav_up;
    std::string nav_down;
    std::string nav_left;
    std::string nav_right;
};

/**
 * @class MapLayout
 * @brief 实现了游戏内地图功能的FTXUI组件。
 */
class MapLayout : public ftxui::ComponentBase {
public:
    explicit MapLayout(Game& game_logic, bool& isShowingFlag);

    ftxui::Element Render() override;

    // 添加 OnEvent 函数的声明，并使用 override 关键字确保正确覆盖基类方法
    bool OnEvent(ftxui::Event event) override;

    // 提供一个公共方法用于重置状态
    void resetState();

private:
    void initializeLocations();
    void travelBy(const std::string& method);

    Game& game_logic_;
    bool& isShowingFlag_;

    // --- UI状态 ---
    // 0: 地图浏览模式, 1: 出行方式确认弹窗
    int viewMode_ = 0;
    std::string selectedLocationId_;

    // --- 地图数据 ---
    std::map<std::string, MapLocation> locations_;

    // --- 子组件 ---
    ftxui::Component main_renderer_;
    ftxui::Component buttonTaxi_;
    ftxui::Component buttonWalk_;
    ftxui::Component buttonCancelTravel_;
    ftxui::Component buttonExit_;
    ftxui::Component container_;
    ftxui::Component dialogContainer_;
};

#endif // MAPLAYOUT_H