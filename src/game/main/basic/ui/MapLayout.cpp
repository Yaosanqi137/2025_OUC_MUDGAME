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
    locations_["home"]         = {"家",         "家",         80, 42, "cafe", "", "", ""};
    locations_["cafe"]         = {"咖啡馆",     "咖啡馆",     80, 32, "store", "home", "", ""};
    locations_["store"]        = {"商店",       "商店",       80, 22, "arena", "cafe", "gym", "pharmacy"};
    locations_["gym"]          = {"拳击馆",     "拳击馆",     40, 22, "construction", "", "", "store"};
    locations_["construction"] = {"工地",       "工地",       40, 12, "", "gym", "", ""};
    locations_["arena"]        = {"比赛场地",   "比赛场地",   80,  5, "", "store", "", ""};
    locations_["pharmacy"]     = {"药店",       "药店",     120, 22, "", "", "store", ""};
}

// 新增：处理购买命令的函数
void MapLayout::processBuyCommand(const std::string& itemName) {
    Player& player = game_logic_.getPlayer();
    const std::string& currentLoc = player.getLocation();
    int price = 0;
    std::string itemDesc;

    // 根据当前场景和商品名，匹配价格和描述
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

    // 检查积蓄是否足够
    if (player.getSavings() < price) {
        game_logic_.addMessage("钱不够买 " + itemName + " ，需要 " + std::to_string(price) + " 元，你只有 " + std::to_string(player.getSavings()) + " 元。");
        return;
    }

    // 扣减积蓄并提示
    player.addSavings(-price);
    game_logic_.addMessage("成功购买 " + itemName + " " + itemDesc + "，花费 " + std::to_string(price) + " 元，剩余积蓄：" + std::to_string(player.getSavings()) + " 元。");
    // （可选）这里可以添加“物品加入背包/冰箱”的逻辑
}

void MapLayout::travelBy(const std::string& method) {
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
            game_logic_.getDialog().addMessage("你发现你没钱坐车了！", "你掏出钱包一看，发现只剩下 " + std::to_string(player.getSavings()) + " 元了");
            viewMode_ = 0;
            return;
        }
    } else if (method == "walk") {
        player.addHunger(-5);
        player.addFatigue(-3);
        GameTime::addMinute(distWalkTime(rng));
        game_logic_.getDialog().addMessage("", "虽然有点累，但是你还是选择了步行前往了 " + destination.name);
    }

    // 场景交互：进入场景后触发对话/商品提示
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
        game_logic_.getDialog().addMessage("", "角落里的自动售货机可以购买补充能量的物品：");
        game_logic_.getDialog().addMessage("", "【商品】能量饮料（饱食度+5 体力+10 价格18元）");
        game_logic_.getDialog().addMessage("", "【商品】巧克力棒（饱食度+5 体力+5 价格12元）");
        game_logic_.getDialog().addMessage("", "【商品】蛋白质棒（饱食度+15 价格18元）");
    } else if (destination.name == "比赛场地") {
        game_logic_.getDialog().addMessage("", "你来到了比赛场地，这里经常举办各种拳击比赛");
        game_logic_.getDialog().addMessage("", "看着上面的擂台，你不由得心潮澎湃");
    } else if (destination.name == "商店") {
        game_logic_.getDialog().addMessage("", "你来到了商店，这里可以买到各种生活用品和食物");
        game_logic_.getDialog().addMessage("商店老板", "欢迎光临，有什么需要的吗？吃的喝的都可以在这里买哦");
        game_logic_.getDialog().addMessage("", "【商品】鸡肉（饱食度+20 血量+5 价格12元）");
        game_logic_.getDialog().addMessage("", "【商品】猪肉（饱食度+20 血量+5 价格12元）");
        game_logic_.getDialog().addMessage("", "【商品】牛肉（饱食度+20 血量+5 价格12元）");
        game_logic_.getDialog().addMessage("", "【商品】苏打水（饱食度+9 价格6元）");
        game_logic_.getDialog().addMessage("", "【商品】能量饮料（饱食度+5 体力+10 价格14元）");
        game_logic_.getDialog().addMessage("", "【商品】冷冻披萨（血量+10 饱食度+17 价格9元）");
        game_logic_.getDialog().addMessage("", "【提示】购买的食物将存放在冰箱中，每种食物上限为7个");
    } else if (destination.name == "药店") {
        game_logic_.getDialog().addMessage("", "你来到了药店，这里可以买到药品和营养品");
        game_logic_.getDialog().addMessage("药店小姐", "你好呀，请问哪里不舒服呢？");
        game_logic_.getDialog().addMessage("", "【商品】创伤恢复包（战败受伤时直接恢复伤病状态 3个 价格50元）");
        game_logic_.getDialog().addMessage("", "【商品】力量属性强化剂（提升属性值+1 单价200元）");
        game_logic_.getDialog().addMessage("", "【商品】耐力属性强化剂（提升属性值+1 单价200元）");
        game_logic_.getDialog().addMessage("", "【商品】敏捷属性强化剂（提升属性值+1 单价200元）");
        game_logic_.getDialog().addMessage("", "【警告】使用属性强化剂会导致能力下降速度提升一倍（永久效果）");
        game_logic_.getDialog().addMessage("", "【商品】技能点药剂（获得1个技能点 价格100元）");
    } else if (destination.name == "咖啡馆") {
        game_logic_.getDialog().addMessage("", "你来到了咖啡馆，这里有各种美味的饮品和甜点");
        game_logic_.getDialog().addMessage("", "此时，一个可爱的，穿着女仆装的少女走了过来");
        game_logic_.getDialog().addMessage("女仆", "你好呀，欢迎来到女仆咖啡馆，请问需要点什么吗？");
        game_logic_.getDialog().addMessage("女仆", "我们这里有这里最好吃最好喝的甜点和饮品哦~");
        game_logic_.getDialog().addMessage("", "【商品】咖啡（88元） | 巧克力可颂（38元） | 抹茶可颂（38元）");
        game_logic_.getDialog().addMessage("", "【商品】巧克力巴斯克（38元） | 抹茶巴斯克（38元）");
    }

    // 首次旅行帮助信息
    if (player.isFirstTimeTravel()) {
        game_logic_.getDialog().addMessage("【系统提示】", "这是你第一次旅行！");
        game_logic_.getDialog().addMessage("【系统提示】", "在任意场景中输入/use可以获取该场景的使用帮助。");
        game_logic_.getDialog().addMessage("【系统提示】", "输入/buy [商品名] 可以购买物品，例如: /buy 鸡肉");
        player.setFirstTimeTravel(false);
    }

    player.setLocation(destination.name);
    isShowingFlag_ = false;
}

Element MapLayout::Render() {
    auto legend = hbox({
        text("图例: "),
        text("■") | color(Color::Green) | bold, text(" 已选择  "),
        text("■") | color(Color::Yellow) | bold,