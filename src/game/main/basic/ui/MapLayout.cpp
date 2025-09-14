#include "MapLayout.h"
#include "../Game.h"
#include "../../class/entity/Player.h"
#include "FTXUI/dom/elements.hpp"
#include "FTXUI/screen/string.hpp"
#include "FTXUI/dom/canvas.hpp"
#include "FTXUI/component/component.hpp"

using namespace ftxui;

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

MapLayout::MapLayout(Game& game_logic) : game_logic_(game_logic) {
    initializeLocations();

    buttonTaxi_ = Button("计程车 (15元)", [this] { travelBy("taxi"); });
    buttonWalk_ = Button("步行", [this] { travelBy("walk"); });
    buttonCancelTravel_ = Button("取消", [this] { viewMode_ = 0; });
    buttonExit_ = Button("[ 退出地图 ]", [this] { hide(); });

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

bool MapLayout::OnEvent(Event event) {
    if (!isShowing_) {
        return false;
    }

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
    // 增加目标地点不存在的容错处理
    auto dest_it = locations_.find(selectedLocationId_);
    if (dest_it == locations_.end()) {
        game_logic_.addMessage("【错误】目标地点不存在，请重新选择！");
        viewMode_ = 0;
        return;
    }
    const auto& destination = dest_it->second;

    if (method == "taxi") {
        if (player.getSavings() < 15) {
            game_logic_.addMessage("钱不够乘坐计程车，需要15元。");
            viewMode_ = 0;
            return;
        }
        player.addSavings(-15);
        game_logic_.addMessage("你乘坐计程车前往" + destination.name + "，花费了15元。");
    } else if (method == "walk") {
        player.addHunger(-5);
        player.addFatigue(-3);
        game_logic_.addMessage("你选择步行前往" + destination.name + "。");
    }

    // 场景商品与价格信息
    if (destination.id == "cafe") {  // 咖啡馆商品保持不变
        game_logic_.addMessage("咖啡馆内飘着咖啡的香气，环境十分舒适。");
        game_logic_.addMessage("【商品】咖啡（88元） | 巧克力可颂（38元） | 抹茶可颂（38元）");
        game_logic_.addMessage("【商品】巧克力巴斯克（38元） | 抹茶巴斯克（38元）");
    } else if (destination.id == "store") {  // 商店商品
        game_logic_.addMessage("商店里商品琳琅满目，你可以在这里购买需要的物品。");
        game_logic_.addMessage(" 【商品】鸡肉（饱食度+20 血量+5 价格12元）");
        game_logic_.addMessage(" 【商品】猪肉（饱食度+20 血量+5 价格12元）");
        game_logic_.addMessage(" 【商品】牛肉（饱食度+20 血量+5 价格12元）");
        game_logic_.addMessage(" 【商品】牛肉（饱食度+20 血量+5 价格12元）");
        game_logic_.addMessage("【商品】苏打水（饱食度+9 价格6元）");
        game_logic_.addMessage("【商品】能量饮料（饱食度+5 体力+10 价格14元）");
        game_logic_.addMessage("【商品】冷冻披萨（血量+10 饱食度+17 价格9元）");
        game_logic_.addMessage("【提示】购买的食物将存放在冰箱中，每种食物上限为7个");
    } else if (destination.id == "pharmacy") {  // 药店商品
        game_logic_.addMessage("药店里很安静，药剂师正在整理药品。");
        game_logic_.addMessage("【商品】创伤恢复包（战败受伤时直接恢复伤病状态 3个 价格50元）");
        game_logic_.addMessage("【商品】力量属性强化剂（提升属性值+1 单价200元）");
        game_logic_.addMessage("【商品】耐力属性强化剂（提升属性值+1 单价200元）");
        game_logic_.addMessage("【商品】敏捷属性强化剂（提升属性值+1 单价200元）");
        game_logic_.addMessage("【警告】使用属性强化剂会导致能力下降速度提升一倍（永久效果）");
        game_logic_.addMessage("【商品】技能点药剂（获得1个技能点 价格100元）");
    } else if (destination.id == "gym") {  // 健身房自动售货机
        game_logic_.addMessage("拳击馆里充满了汗水的味道，许多人在刻苦训练。");
        game_logic_.addMessage("角落里的自动售货机可以购买补充能量的物品：");
        game_logic_.addMessage("【商品】能量饮料（饱食度+5 体力+10 价格18元）");
        game_logic_.addMessage("【商品】巧克力棒（饱食度+5 体力+5 价格12元）");
        game_logic_.addMessage("【商品】蛋白质棒（饱食度+15 价格18元）");
    } else if (destination.id == "construction") {
        game_logic_.addMessage("工地上机器声不断，工人们正在辛勤工作。");
    } else if (destination.id == "arena") {
        game_logic_.addMessage("比赛场地灯火通明，似乎即将有精彩的比赛。");
    } else if (destination.id == "home") {
        game_logic_.addMessage("回到家中，你感到一阵放松。");
        game_logic_.addMessage("冰箱里存放着你购买的食物，打开看看吧。");
    }

    // 首次旅行帮助信息
    if (player.isFirstTimeTravel()) {
        game_logic_.addMessage("【系统提示】这是你第一次旅行！");
        game_logic_.addMessage("【系统提示】在任意场景中输入/use可以获取该场景的使用帮助。");
        game_logic_.addMessage("【系统提示】输入/buy [商品名] 可以购买物品，例如: /buy 肉");
        player.setFirstTimeTravel(false);
    }

    player.setLocation(destination.name);
    hide();
}

Element MapLayout::Render() {
    if (!isShowing_) {
        return emptyElement();
    }

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
            if (!navId.empty() && locations_.count(navId)) {
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

void MapLayout::show() {
    isShowing_ = true;
    viewMode_ = 0;

    std::string player_loc = game_logic_.getPlayer().getLocation();
    if (locations_.count(player_loc) > 0) {
        selectedLocationId_ = player_loc;
    } else {
        selectedLocationId_ = "home";
    }
}

void MapLayout::hide() {
    isShowing_ = false;
}

bool MapLayout::isShowing() const {
    return isShowing_;
}