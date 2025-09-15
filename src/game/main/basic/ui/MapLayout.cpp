#include "MapLayout.h"
#include "../Game.h"
#include "../GameTime.h"
#include "../../class/entity/Player.h"
#include "FTXUI/dom/elements.hpp"
#include "FTXUI/screen/string.hpp"
#include "FTXUI/dom/canvas.hpp"
#include "FTXUI/component/component.hpp"

#include <random>

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
    locations_["pharmacy"]     = {"药店",     "药店", 120, 22, "", "", "store", ""};
}

void MapLayout::travelBy(const std::string& method) {
    std::mt19937 rng(std::random_device{}());
    std::uniform_int_distribution<int> distWalkTime(10, 25); // 步行时间10-20分钟
    std::uniform_int_distribution<int> distTaxiTime(4, 10);  // 计程车时间5-10分钟

    Player& player = game_logic_.getPlayer();
    const auto& destination = locations_.at(selectedLocationId_);

    if (method == "taxi") {
        if (player.getSavings() >= 15) {
            player.addSavings(-15);
            GameTime::addMinute(distTaxiTime(rng));
            game_logic_.getDialog().addMessage("", "你乘坐计程车前往了 " + destination.name + " ，花费了15元");
        } else {
            game_logic_.getDialog().addMessage("", "你掏出钱包一看，发现只剩下 " + std::to_string((int)player.getSavings()) + " 元了");
        }
    } else if (method == "walk") {
        player.addHunger(-5);
        player.addFatigue(-3);
        GameTime::addMinute(distWalkTime(rng));
        game_logic_.getDialog().addMessage("", "虽然有点累，但是你还是选择了步行前往了 " + destination.name);
    }

    if (destination.name == "家") {
        game_logic_.getDialog().addMessage("", "你回到了家，闻到家里的空气，让你感到放松");
        game_logic_.getDialog().addMessage("", "想吃点零食休息一下吗？");
    } else if (destination.name == "工地") {
        game_logic_.getDialog().addMessage("", "你来到了工地，这里有很多体力活可以做");
        game_logic_.getDialog().addMessage("包工头", "喂！小子，看什么呢，你也想搬砖吗？");
        game_logic_.getDialog().addMessage("包工头", "如果你想打工，可以试试/work命令");
    } else if (destination.name == "拳击馆") {
        game_logic_.getDialog().addMessage("", "你来到了拳击馆，看到几个拳击手正在挥汗如雨的训练");
        game_logic_.getDialog().addMessage("教练", "准备练拳！赶紧输入/train指令吧！");
    } else if (destination.name == "比赛场地") {
        game_logic_.getDialog().addMessage("", "你来到了比赛场地，这里经常举办各种拳击比赛");
        game_logic_.getDialog().addMessage("", "看着上面面的擂台，你不由得心潮澎湃");
    } else if (destination.name == "商店") {
        game_logic_.getDialog().addMessage("", "你来到了商店，这里可以买到各种生活用品和食物");
        game_logic_.getDialog().addMessage("商店老板", "欢迎光临，有什么需要的吗？吃的喝的都可以在这里买哦");
    } else if (destination.name == "药店") {
        game_logic_.getDialog().addMessage("", "你来到了药店，这里可以买到药品和营养品");
        game_logic_.getDialog().addMessage("药店小姐", "你好呀，请问哪里不舒服呢？");
    } else if (destination.name == "咖啡馆") {
        game_logic_.getDialog().addMessage("", "你来到了咖啡馆，这里有各种美味的饮品和甜点");
        game_logic_.getDialog().addMessage("","此时，一个可爱的，穿着女仆装的少女走了过来");
        game_logic_.getDialog().addMessage("女仆", "你好呀，欢迎来到女仆咖啡馆，请问需要点什么吗？");
        game_logic_.getDialog().addMessage("女仆", "我们这里有这里最好吃最好喝的甜点和饮品哦~");
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
        std::string extraText;

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

    // 创建右下角的出行方式选择区域
    Element travelDialog = text("");
    if (viewMode_ == 1) {
        auto& dest = locations_.at(selectedLocationId_);
        travelDialog = vbox({
            text("前往 " + dest.name) | bold | color(Color::Yellow),
            hbox({
                buttonTaxi_->Render() | size(WIDTH, LESS_THAN, 22),
                text(" "),
                buttonWalk_->Render() | size(WIDTH, LESS_THAN, 20),
                text(" "),
                buttonCancelTravel_->Render() | size(WIDTH, LESS_THAN, 20)
            })
        }) | border | size(WIDTH, LESS_THAN, 65) | size(HEIGHT, LESS_THAN, 10);
    }

    auto mapElement = vbox({
        legend | center,
        separator(),
        hbox({
            ftxui::canvas(std::move(canvas)) | flex_grow,
            viewMode_ == 1 ? vbox({
                filler(),
                travelDialog
            }) : filler()
        }) | flex,
        separator(),
        vbox({
            text("使用 [↑↓←→] 移动选择, [Home] 确认, [↑↓]键选择出行方式或退选") | color(Color::GrayLight),
            text("如果选择[计程车]出行，则需要花费15元并流逝4~10分钟") | color(Color::GrayLight),
            text("选择[步行]则不花钱但流逝10~25分钟，并消耗5点饥饿值和3点体力") | color(Color::GrayLight),
            text("注：如果没有足够的钱打车，则只能选择步行") | color(Color::GrayLight),
            separator(),
            buttonExit_->Render() | center
        })
    });

    return window(text(" 地图 ") | bold, mapElement) | clear_under;
}
