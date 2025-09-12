#include "GameLayout.h"
#include "BagLayout.h"
#include "ShopLayout.h"
#include "UserInfoLayout.h"
#include "PhoneLayout.h"
#include "MapLayout.h"
#include "SettingsLayout.h"
#include "../Game.h"
#include "../Dialog.h"
#include "../StoryController.h"
#include "../../class/entity/Player.h"

#include "FTXUI/component/screen_interactive.hpp"
#include "FTXUI/dom/elements.hpp"
#include "FTXUI/screen/string.hpp"

using namespace ftxui;

/**
 * @brief GameLayout的构造函数。
 * @details 在此初始化所有UI组件，并设置它们的交互逻辑。
 *          关键在于，所有组件都作为成员变量，保证了其生命周期与GameLayout实例一致。
 */
GameLayout::GameLayout(Game& game_logic) : game_logic_(game_logic),
                                           animationStartTime_(std::chrono::steady_clock::now()) {
    // --- 初始化所有 Layout，并传入对应的可见性标志 ---
    bagLayout_ = Make<BagLayout>(game_logic_, showBag_);
    mapLayout_ =  Make<MapLayout>(game_logic_, showMap_);
    shopLayout_ = Make<ShopLayout>(game_logic_, showShop_);
    infoLayout_ = Make<UserInfoLayout>(game_logic_, showInfo_);
    settingsLayout_ = Make<SettingsLayout>(game_logic_, showSettings_);

    phoneLayout_ = Make<PhoneLayout>(
        game_logic_,
        showPhone_,
        [this] { // onMapClick
            showPhone_ = false;
            if (auto* mapPtr = dynamic_cast<MapLayout*>(mapLayout_.get())) {
                mapPtr->resetState(); // 调用派生类的特定方法
            }
            showMap_ = true;
        },
        [this] { // onShopClick
            showPhone_ = false;
            showShop_ = true;
        },
        [this] { // onInfoClick
            showPhone_ = false;
            showInfo_ = true;
        }
    );

    // -- 对话历史渲染器 --
    auto mainViewRenderer = Renderer([this] {
        // 获取打字动画效果速度
        const int typewriter_speed_ms = game_logic_.getConfig().getTypewriterSpeed();

        const auto& messages = game_logic_.getDialog().getHistory();

        // -- 动画状态重置逻辑 --
        // 检查数据源是否已被重置，如果是，则同步UI的动画状态。
        if (game_logic_.getDialog().historyWasClearedAndConsume()) {
            if (!messages.empty()) {
                // 如果历史记录被清空后又有了新内容，则从头开始播放动画。
                currentMessageIndex_ = 0;
                animationStartTime_ = std::chrono::steady_clock::now();
            } else {
                // 如果历史记录被清空后是空的，则只重置索引。
                currentMessageIndex_ = 0;
            }
        }

        // -- 动画状态推进逻辑 --
        // 只有当有消息且当前动画索引有效时才进行判断
        if (!messages.empty() && currentMessageIndex_ < messages.size()) {
            const auto& currentMsg = messages[currentMessageIndex_];
            auto contentSize = Utf8ToGlyphs(currentMsg.content).size();

            // 计算完成当前消息动画所需的总时间（打字时间 + 结束后延迟）
            auto typingDuration = std::chrono::milliseconds(contentSize * typewriter_speed_ms);
            auto postDelay = std::chrono::milliseconds(500);
            auto totalAnimationTime = typingDuration + postDelay;

            // 检查自当前动画开始以来，是否已经过了所需的总时间
            if (std::chrono::steady_clock::now() - animationStartTime_ > totalAnimationTime) {
                // 如果动画已结束，并且后面还有更多消息，则准备播放下一条
                if (currentMessageIndex_ < messages.size() - 1) {
                    currentMessageIndex_++;
                    // 为下一条消息的动画重置计时器
                    animationStartTime_ = std::chrono::steady_clock::now();
                }
            }
        } else if (!messages.empty()) {
            // 如果消息列表不为空，但索引越界，则修正索引到最后一条消息
            currentMessageIndex_ = messages.size() - 1;
        }

        // -- 渲染逻辑 --
        Elements messageElements;
        size_t total = messages.size();
        size_t end = total > static_cast<size_t>(scrollIndex_) ? total - scrollIndex_ : 0;

        for (size_t i = 0; i < end; ++i) {
            // 只处理和渲染那些已经播放完毕或正在播放的消息。
            // 完全跳过未来的消息，避免它们的 "who" 提前显示。
            if (i > currentMessageIndex_) {
                continue; // 跳过此循环的剩余部分
            }

            const auto& msg = messages[i];
            auto whoColor = (msg.who == game_logic_.getPlayer().getName()) ? Color::Green : Color::Cyan;
            auto glyphs = Utf8ToGlyphs(msg.content);
            size_t contentSize = glyphs.size();
            size_t shownChars = 0;

            if (i < currentMessageIndex_) {
                // 已播放完毕的消息，完整显示
                shownChars = contentSize;
            } else { // 循环开头已有 i > current_message_index_ 判断，故此处必为 i == current_message_index_
                // 正在播放的当前消息，根据独立的动画计时器计算显示长度
                auto elapsedMS = std::chrono::duration_cast<std::chrono::milliseconds>(
                    std::chrono::steady_clock::now() - animationStartTime_
                ).count();
                // 使用配置的速度值。增加一个保护，防止速度为0时除零错误。
                if (typewriter_speed_ms > 0) {
                    shownChars = std::min(contentSize, static_cast<size_t>(elapsedMS / typewriter_speed_ms));
                } else {
                    shownChars = contentSize; // 速度为0或负数时立即显示
                }
            }
            // (i > current_message_index_) 的未来消息，shownChars 保持为0，不显示

            std::string shownContent;
            for (size_t j = 0; j < shownChars && j < glyphs.size(); ++j) {
                shownContent += glyphs[j];
            }
            if ((i == currentMessageIndex_) && (shownChars < contentSize)) {
               shownContent += "_";
            }
            messageElements.push_back(hbox({
                text(msg.who) | bold | color(whoColor),
                text(msg.who.empty() ? "" : ": "),
                paragraph(shownContent) | flex
            }));
        }
        if (!messageElements.empty()) {
            messageElements.back() |= focus;
        }

        return window(text(" 对话记录 "), vbox(messageElements) | flex | yframe);
    });

    interactiveMainView_ = CatchEvent(mainViewRenderer, [this](const Event& e) {
        const auto& messages = game_logic_.getDialog().getHistory();
        int max_offset = static_cast<int>(messages.size()) - 1;
        if (max_offset < 0) max_offset = 0;
        if (e == Event::ArrowUp || e == Event::PageUp) {
            scrollIndex_ = std::min(scrollIndex_ + (e == Event::PageUp ? 5 : 1), max_offset);
            return true;
        }
        if (e == Event::ArrowDown || e == Event::PageDown) {
            scrollIndex_ = std::max(scrollIndex_ - (e == Event::PageDown ? 5 : 1), 0);
            return true;
        }
        if (e == Event::End) { scrollIndex_ = 0; return true; }
        if (e == Event::Home) { scrollIndex_ = max_offset; return true; }
        return false;
    });

    // -- 右侧功能菜单 --
    auto buttonPhone = Button(" 我的手机 ", [&] { showPhone_ = true; }, ButtonOption::Animated());
    auto buttonSettings = Button(" 游戏设置 ", [&] {
        if(auto* settingsPtr = dynamic_cast<SettingsLayout*>(settingsLayout_.get())) { settingsPtr->loadSettings(); }
        showSettings_ = true; }, ButtonOption::Animated());
    auto buttonBag = Button("   背包  ", [&] { showBag_ = true; }, ButtonOption::Animated());
    auto buttonSchedule = Button(" 我的日程 ", []{}, ButtonOption::Animated());
    navigationContainer_ = Container::Vertical({
        buttonPhone,
        buttonSettings,
        buttonBag,
        buttonSchedule,
    });

    // -- 输入组件 --
    commandInputComponent_ = Input(&commandInputStr_, "输入指令或对话...", {
        .on_enter = [&] {
            game_logic_.getDialog().processPlayerInput(commandInputStr_);
            commandInputStr_.clear();
        }
    });

    textInputComponent_ = Input(&textInputStr_, "...", {
        .on_enter = [&] {
            auto requestOpt = game_logic_.getCurrentInputRequest();
            if (!requestOpt) return;

            bool ruleMatched = false;
            for (const auto& rule : requestOpt->rules) {
                if (rule.condition(textInputStr_)) {
                    rule.action(game_logic_, textInputStr_);
                    ruleMatched = true;
                    break;
                }
            }
            if (!ruleMatched && requestOpt->onTextSubmitDefault) {
                requestOpt->onTextSubmitDefault(game_logic_, textInputStr_);
            }
            textInputStr_.clear();
        }
    });

    choiceContainer_ = Container::Vertical({});

    inputArea_ = Container::Tab(
        {
            commandInputComponent_,
            textInputComponent_,
            choiceContainer_,
        },
        &selectedInputMode_
    );

    // 将所有主界面组件组合到一个单独的容器中
    auto main_layout_renderer = Renderer(
        Container::Vertical({interactiveMainView_, navigationContainer_, inputArea_}),
        [&] {
            Elements leftChildren;
            leftChildren.push_back(interactiveMainView_->Render() | vscroll_indicator | yframe | flex);
            if (showPlayerStatus_) {
                leftChildren.push_back(
                    window(text(" 玩家状态 "),
                           hbox({
                               text("生命值: " + std::to_string(game_logic_.getPlayer().getHealth())) | color(Color::Green) | flex,
                               separator(),
                               text("体力: " + std::to_string(game_logic_.getPlayer().getFatigue())) | color(Color::Yellow) | flex,
                               separator(),
                               text("饥饿值: " + std::to_string(game_logic_.getPlayer().getHunger())) | color(Color::RedLight) | flex
                           }))
                );
            }
            auto leftPanel = vbox(leftChildren) | flex;

            Element rightPanel = emptyElement();
            if (showSidePanels_) {
                rightPanel = window(text(" 功能菜单 "), navigationContainer_->Render()) | size(WIDTH, EQUAL, 22);
            }

            Element mainContent = hbox({ leftPanel, rightPanel });

            Element footer = emptyElement();
            if (showFooter_) {
                // lastInputPrompt_ 是在 Render() 函数中更新的，这里通过引用捕获来获取最新值。
                footer = window(text(lastInputPrompt_), inputArea_->Render());
            }

            return vbox({ mainContent | flex, footer });
        }
    );

    // --- 创建最终的顶层容器 ---
    topLevelContainer_ = Container::Stacked({
        // 底层：主游戏布局
        Maybe(main_layout_renderer, &showMainUI_),
        // 顶层：所有可能的弹出窗口
        Maybe(bagLayout_, &showBag_),
        Maybe(phoneLayout_, &showPhone_),
        Maybe(mapLayout_, &showMap_),
        Maybe(shopLayout_, &showShop_),
        Maybe(infoLayout_, &showInfo_),
        Maybe(settingsLayout_, &showSettings_)
    });

    // 将这个顶层容器作为 GameLayout 的子组件
    Add(topLevelContainer_);
}

/**
 * @brief 渲染函数，FTXUI每一帧都会调用。
 * @details 负责根据当前游戏状态同步UI，并组合所有子组件来构建最终的界面布局。
 */
Element GameLayout::Render() {
    // 使用 Maybe 后，不再需要在 Render 函数的开头检查每个覆盖层是否显示。
    // FTXUI 会自动处理。只需要考虑渲染主布局即可。

    // 更新剧情控制器
    game_logic_.getStoryController().update();

    // 状态同步：根据Game状态切换Tab的显示，并动态更新内容
    GameState state = game_logic_.getCurrentState();
    auto requestOpt = game_logic_.getCurrentInputRequest();
    lastInputPrompt_ = " 指令 ";

    switch(state) {
        case GameState::AwaitingTextInput:
            selectedInputMode_ = 1;
            if (requestOpt) lastInputPrompt_ = requestOpt->prompt;
            break;
        case GameState::AwaitingChoice:
            selectedInputMode_ = 2;
            if (requestOpt) {
                lastInputPrompt_ = requestOpt->prompt;
                // 动态更新选项按钮
                if (choiceContainer_->ChildCount() != requestOpt->choices.size()) {
                    choiceContainer_->DetachAllChildren();
                    for (int i = 0; i < requestOpt->choices.size(); ++i) {
                         choiceContainer_->Add(
                             Button(requestOpt->choices[i], [i, choice = requestOpt->choices[i], onSelect = requestOpt->onChoiceSelect] {
                                 onSelect(i, choice);
                             }, ButtonOption::Animated())
                         );
                    }
                }
            }
            break;
        case GameState::InStory:
            selectedInputMode_ = -1;
            lastInputPrompt_ = "正在播放剧情...";
            break;
        default: // GameState::InGame 或其他
            selectedInputMode_ = 0;
            break;
    }

    showMainUI_ = !isAnyPopupActive();
    showSidePanels_ = (state == GameState::InGame) && !isAnyPopupActive();
    showPlayerStatus_ = (state == GameState::InGame) && !isAnyPopupActive();
    showFooter_ = (state != GameState::InStory) && !isAnyPopupActive();

    // 构建最外层的静态框架 (header)
    auto header = hbox({
        text("   拳王之路   ") | bold | color(Color::Red),
        filler(),
        text("当前位置: " + game_logic_.getPlayer().getLocation() + " ") | color(Color::Yellow),
    }) | border;

    Element mainContent = topLevelContainer_->Render();

    Element screen_layout = vbox({
        header,
        mainContent | flex,
    });

    return screen_layout;
}

bool GameLayout::isAnyPopupActive() const {
    return showBag_ || showPhone_ || showMap_ || showShop_ || showInfo_ || showSettings_;
}

GameLayout::~GameLayout() = default;
