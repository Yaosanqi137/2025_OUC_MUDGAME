#include "MapLayout.h"
#include "../Game.h"
#include "../GameTime.h"
#include "../../class/entity/Player.h"
#include "FTXUI/dom/elements.hpp"
#include "FTXUI/screen/string.hpp"
#include "FTXUI/dom/canvas.hpp"
#include "FTXUI/component/component.hpp"

#include <random>
#include <sstream>
#include <vector>

using namespace ftxui;

// 构造函数，参数与头文件一致，初始化成员变量
MapLayout::MapLayout(Game& game_logic)
    : game_logic_(game_logic), isShowing_(false), viewMode_(0) {
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

    container_ = Container::Vertical({
        dialogContainer_,
        buttonExit_,
    });
    Add(container_);
}

void MapLayout::resetState() {
    viewMode_ = 0;
    const std::string& player_loc_name = game_logic_.getPlayer().getLocation();
    std::string start_id = "home";
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

    canvas.DrawText(startX, startY, std::string(tl) + std::string(boxWidth - 2, *h) + std::string(tr), color);
    canvas.DrawText(startX, startY + 2, std::string(bl) + std::string(boxWidth - 2, *h) + std::string(br), color);
    canvas.DrawText(startX, startY + 1, *v, color);
    canvas.DrawText(startX + boxWidth - 1, startY + 1, *v, color);
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
    locations_["home"]         = {"home",         "家",         80, 42, "cafe", "", "", ""};
    locations_["cafe"]         = {"cafe",         "咖啡馆",     80, 32, "store", "home", "", ""};
    locations_["store"]        = {"store",        "商店",       80, 22, "arena", "cafe", "gym", "pharmacy"};
    locations_["gym"]          = {"gym",          "拳击馆",     40, 22, "construction", "", "", "store"};
    locations_["construction"] = {"construction", "工地",       40, 12, "", "gym", "", ""};
    locations_["arena"]        = {"arena",        "比赛场地",   80,  5, "", "store", "", ""};
    locations_["pharmacy"]     = {"pharmacy",     "药店",     120, 22, "", "", "store", ""};
}

void MapLayout::processBuyCommand(const std::string& itemName) {
    Player& player = game_logic_.getPlayer();
    const std::string& currentLoc = player.getLocation();
    int price = 0;
    std::string itemDesc;

    if (currentLoc == "商店") {
        if (itemName == "鸡肉" || itemName == "猪肉" || itemName == "牛肉") {
            price = 12;
            itemDesc = "饱食度+20 血量+5";
        } else if (itemName == "苏打水") {
            price = 6;
            itemDesc = "饱食度+9";
        } else if (itemName == "能量饮料") {
            price = 14;
            itemDesc = "饱食度+5 体力+10";
        } else if (itemName == "冷冻披萨") {
            price = 9;
            itemDesc = "血量+10 饱食度+17";
        } else {
            game_logic_.addMessage("商店没有 " + itemName + " 这种商品！");
            return;
        }
    } else if (currentLoc == "咖啡馆") {
        if (itemName == "咖啡") {
            price = 88;
            itemDesc = "（提神饮品）";
        } else if (itemName == "巧克力可颂" || itemName == "抹茶可颂" || 
                   itemName == "巧克力巴斯克" || itemName == "抹茶巴斯克") {
            price = 38;
            itemDesc = "（甜点）";
        } else {
            game_logic_.addMessage("咖啡馆没有 " + itemName + " 这种商品！");
            return;
        }
    } else if (currentLoc == "药店") {
        if (itemName == "创伤恢复包") {
            price = 50;
            itemDesc = "（战败受伤时直接恢复伤病状态，含3个）";
        } else if (itemName == "力量属性强化剂" || itemName == "耐力属性强化剂" || 
                   itemName == "敏捷属性强化剂") {
            price = 200;
            itemDesc = "（提升对应属性+1，永久效果，但能力下降速度翻倍）";
        } else if (itemName == "技能点药剂") {
            price = 100;
            itemDesc = "（获得1个技能点）";
        } else {
            game_logic_.addMessage("药店没有 " + itemName + " 这种商品！");
            return;
        }
    } else if (currentLoc == "拳击馆") {
        if (itemName == "能量饮料") {
            price = 18;
            itemDesc = "饱食度+5 体力+10";
        } else if (itemName == "巧克力棒") {
            price = 12;
            itemDesc = "饱食度+5 体力+5";
        } else if (itemName == "蛋白质棒") {
            price = 18;
            itemDesc = "饱食度+15";
        } else {
            game_logic_.addMessage("拳击馆自动售货机没有 " + itemName + " 这种商品！");
            return;
        }
    } else {
        game_logic_.addMessage("当前场景无法购买商品！");
        return;
    }

    if (player.getSavings() < price) {
        game_logic_.addMessage("钱不够买 " + itemName + " ，需要 " + std::to_string(price) + " 元，你只有 " + std::to_string(player.getSavings()) + " 元。");
        return;
    }

    player.addSavings(-price);
    game_logic_.addMessage("成功购买 " + itemName + " " + itemDesc + "，花费 " + std::to_string(price) + " 元，剩余积蓄：" + std::to_string(player.getSavings()) + " 元。");
}

void MapLayout::travelBy(const std::string& method) {
    if (game_logic_.isInBattle()) {
        game_logic_.getDialog().addMessage("<SYSTEM>", "在战斗中不能移动！");
        hide(); 
        return;
    }
    
    std::mt19937 rng(std::random_device{}());
    std::uniform_int_distribution<int> distWalkTime(10, 25);
    std::uniform_int_distribution<int> distTaxiTime(4, 10);

    Player& player = game_logic_.getPlayer();
    auto dest_it = locations_.find(selectedLocationId_);
    if (dest_it == locations_.end()) {
        game_logic_.addMessage("【错误】目标地点不存在，请重新选择！");
        viewMode_ = 0;
        return;
    }
    const auto& destination = dest_it->second;

    if (method == "taxi") {
        if (player.getSavings() >= 15) {
            player.addSavings(-15);
            GameTime::addMinute(distTaxiTime(rng));
            game_logic_.getDialog().addMessage("", "你乘坐计程车前往了 " + destination.name + " ，花费了15元");
        } else {
            game_logic_.getDialog().addMessage("", "你掏出钱包一看，发现只剩下 " + std::to_string(player.getSavings()) + " 元了");
            viewMode_ = 0;
            return;
        }
    } else if (method == "walk") {
        player.addHunger(5);
        player.addFatigue(3);
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
    }
    
    player.setLocation(destination.name);
    hide();
}

Element MapLayout::Render() {
    if (!isShowing()) {
        return text("");
    }

    auto canvas = Canvas(160, 50);
    Color defaultColor = Color::White;

    for (const auto& [id, loc] : locations_) {
        bool isSelected = (id == selectedLocationId_);
        DrawLocationNode(canvas, loc, defaultColor, isSelected);
    }

    for (const auto& [id, loc] : locations_) {
        if (!loc.nav_up.empty()) {
            auto& upLoc = locations_.at(loc.nav_up);
            canvas.DrawLine(loc.x, loc.y, upLoc.x, upLoc.y, '-', defaultColor);
        }
        if (!loc.nav_down.empty()) {
            auto& downLoc = locations_.at(loc.nav_down);
            canvas.DrawLine(loc.x, loc.y, downLoc.x, downLoc.y, '-', defaultColor);
        }
        if (!loc.nav_left.empty()) {
            auto& leftLoc = locations_.at(loc.nav_left);
            canvas.DrawLine(loc.x, loc.y, leftLoc.x, leftLoc.y, '|', defaultColor);
        }
        if (!loc.nav_right.empty()) {
            auto& rightLoc = locations_.at(loc.nav_right);
            canvas.DrawLine(loc.x, loc.y, rightLoc.x, rightLoc.y, '|', defaultColor);
        }
    }

    auto mapElement = canvas.Render() | border;

    auto dialogElement = viewMode_ == 1 
        ? window(text("选择出行方式"), dialogContainer_->Render()) | size(WIDTH, LESS_THAN, 30)
        : emptyElement();

    return vbox({
        text("游戏地图") | bold | hcenter,
        separator(),
        hbox({
            mapElement | flex,
            dialogElement
        }) | flex,
        separator(),
        buttonExit_->Render() | hcenter
    }) | border | clear_under;
}

void MapLayout::show() {
    isShowing_ = true;
    resetState();
}

void MapLayout::hide() {
    isShowing_ = false;
}

bool MapLayout::isShowing() const {
    return isShowing_;
}
