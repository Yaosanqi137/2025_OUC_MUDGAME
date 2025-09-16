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

    buttonTaxi_ = Button("计程车", [this] { travelBy("taxi"); });
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
    locations_["home"]         = {"家",         "家",         80, 42, "cafe", "", "", "", false}; // 家不是第一次来
    locations_["cafe"]         = {"咖啡馆",         "咖啡馆",     80, 32, "store", "home", "", "", true};
    locations_["store"]        = {"商店",        "商店",       80, 22, "arena", "cafe", "gym", "pharmacy", true};
    locations_["gym"]          = {"拳击馆",          "拳击馆",     40, 22, "construction", "", "", "store", true};
    locations_["construction"] = {"工地", "工地",       40, 12, "", "gym", "", "", true};
    locations_["arena"]        = {"比赛场地",        "比赛场地",   80,  5, "", "store", "", "", true};
    locations_["pharmacy"]     = {"药店",     "药店", 120, 22, "", "", "store", "", true};
}

void MapLayout::travelBy(const std::string& method) {

    if (game_logic_.isInBattle()) {
        game_logic_.getDialog().addMessage("<SYSTEM>", "在战斗中不能移动！");
        isShowingFlag_ = false; // 关闭地图界面
        return;
    }
    
    std::mt19937 rng(std::random_device{}());
    std::uniform_int_distribution<int> distWalkTime(10, 25); // 步行时间10-20分钟
    std::uniform_int_distribution<int> distTaxiTime(4, 10);  // 计程车时间5-10分钟

    Player& player = game_logic_.getPlayer();
    auto& destination = locations_.at(selectedLocationId_); // 使用引用以便修改

    // 执行移动逻辑
    bool moveSuccessful = false;
    if (method == "taxi") {
        if (player.getSavings() >= 15) {
            player.addSavings(-15);
            GameTime::addMinute(distTaxiTime(rng));
            game_logic_.getDialog().addMessage("", "你乘坐计程车前往了 " + destination.name + " ，花费了15元");
            moveSuccessful = true;
        } else {
            game_logic_.getDialog().addMessage("", "你掏出钱包一看，发现只剩下 " + std::to_string((int)player.getSavings()) + " 元了");
            viewMode_ = 0; // 回到地图浏览模式
            return;
        }
    } else if (method == "walk") {
        player.addHunger(-5);
        player.addFatigue(-3);
        GameTime::addMinute(distWalkTime(rng));
        game_logic_.getDialog().addMessage("", "虽然有点累，但是你还是选择了步行前往了 " + destination.name);
        moveSuccessful = true;
    }

    if (!moveSuccessful) {
        return;
    }

    // 检查是否是第一次来到这个地点
    bool isFirstVisit = destination.isFirstTo;
    
    // 根据地点显示对应的对话和引导
    if (destination.name == "家") {
        if (isFirstVisit) {
            game_logic_.getDialog().addMessage("", "这里是你的家，你最熟悉的地方");
            game_logic_.getDialog().addMessage("", "在家里你可以通过 /sleep 指令睡觉来恢复体力和生命值");
            game_logic_.getDialog().addMessage("", "也可以通过 /eat 指令吃零食来恢复一点饱食度");
        } else {
            game_logic_.getDialog().addMessage("", "你回到了家，闻到家里的空气，让你感到放松");
            game_logic_.getDialog().addMessage("", "想吃点零食休息一下吗？");
        }
    } else if (destination.name == "工地") {
        if (isFirstVisit) {
            game_logic_.getDialog().addMessage("", "你第一次来到了工地，这里尘土飞扬，机器轰鸣");
            game_logic_.getDialog().addMessage("", "工地是赚钱的好地方，虽然辛苦但是收入还算可以");
            game_logic_.getDialog().addMessage("包工头", "新来的？看起来不错啊，想打工吗？");
            game_logic_.getDialog().addMessage("包工头", "我们这里按次计费，每次工作能赚40-80元");
            game_logic_.getDialog().addMessage("包工头", "不过会消耗你的体力和饱食度，要做好准备");
            game_logic_.getDialog().addMessage("包工头", "如果你想打工，可以试试 /work 命令");
        } else {
            game_logic_.getDialog().addMessage("", "你来到了工地，这里有很多体力活可以做");
            game_logic_.getDialog().addMessage("包工头", "喂！小子，看什么呢，你也想搬砖吗？");
            game_logic_.getDialog().addMessage("包工头", "如果你想打工，可以试试/work命令");
        }
    } else if (destination.name == "拳击馆") {
        if (isFirstVisit) {
            game_logic_.getDialog().addMessage("", "你第一次踏进拳击馆，空气中弥漫着汗水和激情的味道");
            game_logic_.getDialog().addMessage("", "各种训练器材整齐排列，拳击手们正在刻苦训练");
            game_logic_.getDialog().addMessage("教练", "新面孔？看起来很有潜力啊！");
            game_logic_.getDialog().addMessage("教练", "想要成为强者就必须经过刻苦的训练");
            game_logic_.getDialog().addMessage("教练", "我们这里有三种训练：力量、敏捷和耐力训练");
            game_logic_.getDialog().addMessage("教练", "使用 /train 命令查看训练帮助，用 /train str、/train ag、/train sta 开始训练");
            game_logic_.getDialog().addMessage("教练", "记住，训练需要消耗金钱、体力和饱食度，但会让你变得更强！");
        } else {
            game_logic_.getDialog().addMessage("", "你来到了拳击馆，看到几个拳击手正在挥汗如雨的训练");
            game_logic_.getDialog().addMessage("教练", "准备练拳！赶紧输入/train指令吧！");
        }
    } else if (destination.name == "比赛场地") {
        if (isFirstVisit) {
            game_logic_.getDialog().addMessage("", "你第一次来到比赛场地，这里是职业拳击手的战场");
            game_logic_.getDialog().addMessage("", "巨大的擂台矗立在中央，周围是观众席");
            game_logic_.getDialog().addMessage("", "你可以感受到这里曾经发生过的激烈战斗");
            game_logic_.getDialog().addMessage("裁判", "新来的选手？想要参加比赛吗？");
            game_logic_.getDialog().addMessage("裁判", "这里有各种级别的对手等着你挑战");
            game_logic_.getDialog().addMessage("裁判", "使用 /enemy show 查看下一个对手，/enemy battle 开始战斗");
            game_logic_.getDialog().addMessage("裁判", "记住，只有足够强大才能在这里生存！");
        } else {
            game_logic_.getDialog().addMessage("", "你来到了比赛场地，这里经常举办各种拳击比赛");
            game_logic_.getDialog().addMessage("", "看着上面面的擂台，你不由得心潮澎湃");
        }
    } else if (destination.name == "商店") {
        if (isFirstVisit) {
            game_logic_.getDialog().addMessage("", "你第一次走进这家商店，货架上摆满了各种商品");
            game_logic_.getDialog().addMessage("", "从食物到生活用品，这里应有尽有");
            game_logic_.getDialog().addMessage("老板", "欢迎来到我的小店！第一次来吧？");
            game_logic_.getDialog().addMessage("老板", "我们这里有各种食物：烤肉、苏打水、能量饮料、冷冻披萨");
            game_logic_.getDialog().addMessage("老板", "价格实惠，质量保证！");
            game_logic_.getDialog().addMessage("老板", "使用 /buy 指令可以打开购买界面");
            game_logic_.getDialog().addMessage("老板", "记得带够钱哦~");
        } else {
            game_logic_.getDialog().addMessage("", "你来到了商店，这里可以买到各种生活用品和食物");
            game_logic_.getDialog().addMessage("老板", "欢迎光临，有什么需要的吗？吃的喝的都可以在这里买哦");
        }
    } else if (destination.name == "药店") {
        if (isFirstVisit) {
            game_logic_.getDialog().addMessage("", "你第一次来到药店，整洁的环境给人专业的感觉");
            game_logic_.getDialog().addMessage("", "各种药品整齐地摆放在货架上");
            game_logic_.getDialog().addMessage("药店小姐", "欢迎光临！第一次来我们药店吗？");
            game_logic_.getDialog().addMessage("药店小姐", "我们这里有各种功效的药品");
            game_logic_.getDialog().addMessage("药店小姐", "回生丹可以恢复生命值，各种药水可以提升属性");
            game_logic_.getDialog().addMessage("药店小姐", "不过要注意，一些药水可能有副作用哦");
            game_logic_.getDialog().addMessage("药店小姐", "使用 /buy 指令可以查看并购买药品");
        } else {
            game_logic_.getDialog().addMessage("", "你来到了药店，这里可以买到药品和营养品");
            game_logic_.getDialog().addMessage("药店小姐", "你好呀，请问哪里不舒服呢？");
        }
    } else if (destination.name == "咖啡馆") {
        if (isFirstVisit) {
            game_logic_.getDialog().addMessage("", "你第一次踏进这家女仆咖啡馆，温馨的装饰让人眼前一亮");
            game_logic_.getDialog().addMessage("", "空气中弥漫着咖啡的香味和甜点的诱人气息");
            game_logic_.getDialog().addMessage("", "此时，一个可爱的，穿着女仆装的少女走了过来");
            game_logic_.getDialog().addMessage("女仆", "欢迎来到女仆咖啡馆！第一次光临吗？");
            game_logic_.getDialog().addMessage("女仆", "我们这里有最特别的女仆咖啡、爱心面包");
            game_logic_.getDialog().addMessage("女仆", "还有我们的招牌——瓦学弟蛋包饭！");
            game_logic_.getDialog().addMessage("女仆", "每一样都是用爱心制作的呢~");
            game_logic_.getDialog().addMessage("女仆", "使用 /buy 指令可以查看菜单并点餐");
            game_logic_.getDialog().addMessage("女仆", "记得要说'请'和'谢谢'哦~");
        } else {
            game_logic_.getDialog().addMessage("", "你来到了咖啡馆，这里有各种美味的饮品和甜点");
            game_logic_.getDialog().addMessage("","此时，一个可爱的，穿着女仆装的少女走了过来");
            game_logic_.getDialog().addMessage("女仆", "你好呀，欢迎来到女仆咖啡馆，请问需要点什么吗？");
            game_logic_.getDialog().addMessage("女仆", "我们这里有这里最好吃最好喝的甜点和饮品哦~");
        }
    }

    // 移动成功后，设置玩家位置并标记不再是第一次访问
    player.setLocation(destination.name);
    if (isFirstVisit) {
        destination.isFirstTo = false;
    }
    
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
                buttonTaxi_->Render() | size(WIDTH, EQUAL, 15),
                text(" "),
                buttonWalk_->Render() | size(WIDTH, EQUAL, 15),
                text(" "),
                buttonCancelTravel_->Render() | size(WIDTH, EQUAL, 15)
            })
        }) | border | size(WIDTH, EQUAL, 50) | size(HEIGHT, LESS_THAN, 10);
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
