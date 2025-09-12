#include "MapLayout.h"
#include "../Game.h"
#include "../../class/entity/Player.h"
#include "FTXUI/dom/elements.hpp"
#include "FTXUI/screen/string.hpp"
#include "FTXUI/dom/canvas.hpp"
#include "FTXUI/component/component.hpp"

using namespace ftxui;

MapLayout::MapLayout(Game& game_logic, bool& isShowingFlag)
    : game_logic_(game_logic), isShowingFlag_(isShowingFlag) {
    initializeLocations();

    buttonTaxi_ = Button("计程车 (15元)", [this] { travelBy("taxi"); });
    buttonWalk_ = Button("步行", [this] { travelBy("walk"); });
    buttonCancelTravel_ = Button("取消", [this] { viewMode_ = 0; });
    buttonExit_ = Button("[ 退出地图 ]", [this] { isShowingFlag_ = false; });

    dialogContainer_ = Container::Vertical({
        buttonTaxi_,
        buttonWalk_,
        buttonCancelTravel_,
    });

    Add(Container::Vertical({
        dialogContainer_,
        buttonExit_,
    }));
}

void MapLayout::resetState() {
    viewMode_ = 0;
    // 找到玩家当前位置对应的地图ID
    const std::string& player_loc_name = game_logic_.getPlayer().getLocation();
    std::string start_id = "home"; // 默认值
    for (const auto& [id, loc] : locations_) {
        if (loc.name == player_loc_name) {
            start_id = id;
            break;
        }
    }
    selectedLocationId_ = start_id;
}

void DrawLocationNode(Canvas& canvas, const MapLocation& loc, Color color, bool isSelected, const std::string& extraText = "") {
    std::string displayName = loc.name + extraText;
    int nameWidth = string_width(displayName);
    int boxWidth = nameWidth + 4;
    int boxHeight = 3;
    int startX = loc.x - boxWidth / 2;
    int startY = loc.y - boxHeight / 2;

    const char *tl, *tr, *bl, *br, *h, *v;
    if (isSelected) {
        tl = "╔"; tr = "╗"; bl = "╚"; br = "╝"; h = "═"; v = "║";
    } else {
        tl = "┌"; tr = "┐"; bl = "└"; br = "┘"; h = "─"; v = "│";
    }

    // 1. 绘制空心方框
    // 1.1 构造顶部和底部边框线
    std::string horizontalLine;
    for (int i = 0; i < boxWidth - 2; ++i) {
        horizontalLine += h;
    }
    canvas.DrawText(startX, startY, std::string(tl) + horizontalLine + std::string(tr), color);
    canvas.DrawText(startX, startY + 2, std::string(bl) + horizontalLine + std::string(br), color);

    // 1.2 绘制侧边框
    canvas.DrawText(startX, startY + 1, v, color);
    canvas.DrawText(startX + boxWidth - 1, startY + 1, v, color);

    // 2. 在方框内部的正确位置单独绘制地名文本
    canvas.DrawText(startX + 2, startY + 1, displayName, color);
}

bool MapLayout::OnEvent(Event event) {
    if (viewMode_ == 1) {
        return dialogContainer_->OnEvent(event);
    }

    if (event == Event::Home) {
        if (locations_[selectedLocationId_].name != game_logic_.getPlayer().getLocation()) {
            viewMode_ = 1;
            dialogContainer_->TakeFocus();
            return true;
        }
    }

    const auto& current_loc = locations_.at(selectedLocationId_);
    std::string next_loc_id;
    if (event == Event::ArrowUp) next_loc_id = current_loc.nav_up;
    else if (event == Event::ArrowDown) next_loc_id = current_loc.nav_down;
    else if (event == Event::ArrowLeft) next_loc_id = current_loc.nav_left;
    else if (event == Event::ArrowRight) next_loc_id = current_loc.nav_right;

    if (!next_loc_id.empty()) {
        selectedLocationId_ = next_loc_id;
        return true;
    }

    return buttonExit_->OnEvent(event);
}

void MapLayout::initializeLocations() {
    locations_["home"]         = {"家",         "家",         80, 42, "cafe", "", "", ""};
    locations_["cafe"]         = {"咖啡馆",         "咖啡馆",     80, 32, "store", "home", "", ""};
    locations_["store"]        = {"商店",        "商店",       80, 22, "arena", "cafe", "gym", "pharmacy"};
    locations_["gym"]          = {"拳击馆",          "拳击馆",     40, 22, "construction", "", "", "store"};
    locations_["construction"] = {"工地", "工地",       40, 12, "", "gym", "", ""};
    locations_["arena"]        = {"比赛场地",        "比赛场地",   80,  5, "", "store", "", ""};
    locations_["pharmacy"]     = {"药店/体检",     "药店/体检", 120, 22, "", "", "store", ""};
}

void MapLayout::travelBy(const std::string& method) {
    Player& player = game_logic_.getPlayer();
    const auto& destination = locations_.at(selectedLocationId_);

    if (method == "taxi") {
        if (player.getSavings() < 15) {
            viewMode_ = 0;
            return;
        }
        player.addSavings(-15);
    } else if (method == "walk") {
        player.addHunger(-5);
        player.addFatigue(-3);
    }

    player.setLocation(destination.name);
    isShowingFlag_ = false;
}

Element MapLayout::Render() {
    auto legend = hbox({
        text("图例: "),
        text("■") | color(Color::Green) | bold, text(" 已选择  "),
        text("■") | color(Color::Yellow) | bold, text(" 当前位置  "),
        text("■") | color(Color::GrayLight), text(" 其他地点"),
    });

    // --- [美化升级 4] ---
    // 显著增大画布尺寸
    auto canvas = Canvas(160, 45);
    const std::string& player_loc_id = game_logic_.getPlayer().getLocation();

    // 先绘制所有连接线，让它们作为背景
    for (const auto& [id, loc] : locations_) {
        for (const auto& navId : {loc.nav_up, loc.nav_down, loc.nav_left, loc.nav_right}) {
            if (!navId.empty() && locations_.contains(navId)) {
                const auto& neighbor = locations_.at(navId);
                // 使用更暗的颜色绘制连接线，以突出节点
                canvas.DrawPointLine(loc.x, loc.y, neighbor.x, neighbor.y, Color::GrayDark);
            }
        }
    }

    // 在连接线之上绘制所有地点节点
    for (const auto& [id, loc] : locations_) {
        Color nodeColor = Color::GrayLight; // 普通地点的默认颜色
        std::string extraText = "";

        if (locations_[id].name == player_loc_id) {
            nodeColor = Color::Yellow;
            extraText = " (你)";
        }
        // 如果当前地点被选中，绿色会覆盖黄色
        if (id == selectedLocationId_) {
            nodeColor = Color::Green;
        }

        // 调用我们新的、更美观的节点绘制函数
        DrawLocationNode(canvas, loc, nodeColor, (id == selectedLocationId_), extraText);
    }

    auto mapElement = vbox({ // 提示框和退出按钮
        legend | center,
        separator(),
        ftxui::canvas(std::move(canvas)) | flex_grow | center,
        separator(),
        hbox({
            text("使用 [↑↓←→] 移动选择, [Home] 确认") | color(Color::GrayLight),
            filler(),
            buttonExit_->Render()
        })
    });

    auto mainMapWindow = window(text(" 地图 ") | bold, mapElement | border);

    if (viewMode_ == 1) {
        auto& dest = locations_.at(selectedLocationId_);
        auto dialog = vbox({
            text("前往 " + dest.name + " ?") | bold | hcenter,
            separator(),
            text("请选择出行方式："),
            text(" "),
            hbox({
                filler(),
                buttonTaxi_->Render(),
                filler(),
                buttonWalk_->Render(),
                filler(),
            }),
            text(" "),
            separator(),
            buttonCancelTravel_->Render() | center
        }) | border | size(WIDTH, LESS_THAN, 50);

        return dbox({ mainMapWindow, dialog | center }) | clear_under;
    }

    return mainMapWindow | clear_under;
}
