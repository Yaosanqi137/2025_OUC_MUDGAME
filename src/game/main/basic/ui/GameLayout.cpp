#include "GameLayout.h"
#include "BagLayout.h"
#include "PhoneLayout.h"
#include "MapLayout.h"
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
    // 初始化背包组件
    bagLayout_ = Make<BagLayout>(game_logic_);
    mapLayout_ =  Make<MapLayout>(game_logic_);

    MapLayout* mapPtr = static_cast<MapLayout*>(mapLayout_.get());
    phoneLayout_ = Make<PhoneLayout>(game_logic_, [this, mapPtr] {
    // 当点击地图按钮时：先隐藏手机，再显示地图
        static_cast<PhoneLayout*>(phoneLayout_.get())->hide();
        mapPtr->show();
    });

    // -- 对话历史渲染器 --
    auto mainViewRenderer = Renderer([this] {
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
            auto typingDuration = std::chrono::milliseconds(contentSize * 20);
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
                shownChars = std::min(contentSize, static_cast<size_t>(elapsedMS / 20));
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
    BagLayout* bagPtr = static_cast<BagLayout*>(bagLayout_.get());
    PhoneLayout* phonePtr = static_cast<PhoneLayout*>(phoneLayout_.get());

    auto buttonPhone = Button(" 我的手机 ", [phonePtr] { phonePtr->show(); }, ButtonOption::Animated());
    auto buttonSettings = Button(" 游戏设置 ", [&] { game_logic_.showGameSettings(); }, ButtonOption::Animated());
    auto buttonBag = Button("   背包  ", [bagPtr] { bagPtr->show(); }, ButtonOption::Animated());
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
    auto mainGameContainer = Container::Vertical({
        interactiveMainView_,
        Maybe(navigationContainer_, &showSidePanels_),
        Maybe(inputArea_, &showFooter_),
    });

    // 使用 Container::Stacked 来管理主界面和背包层，确保背包也能接收事件
    auto topLevelContainer = Container::Stacked({
        mainGameContainer,
        bagLayout_,
        phoneLayout_,
        mapLayout_,
    });

    // 将这个顶层容器作为 GameLayout 的子组件
    Add(topLevelContainer);
}


/**
 * @brief 渲染函数，FTXUI每一帧都会调用。
 * @details 负责根据当前游戏状态同步UI，并组合所有子组件来构建最终的界面布局。
 */
Element GameLayout::Render() {
    // 在每一帧的开始，首先检查是否有任何叠加层处于活动状态。
    // 如果有，则立即渲染该层并返回，从而跳过主界面的所有逻辑和渲染。
    BagLayout* bagPtr = static_cast<BagLayout*>(bagLayout_.get());
    if (bagPtr && bagPtr->isShowing()) {
        return bagLayout_->Render();
    }
    PhoneLayout* phonePtr = static_cast<PhoneLayout*>(phoneLayout_.get());
    if (phonePtr && phonePtr->isShowing()) {
        return phoneLayout_->Render();
    }
    MapLayout* mapPtr = static_cast<MapLayout*>(mapLayout_.get());
    if (mapPtr && mapPtr->isShowing()) {
        return mapLayout_->Render();
    }

    // 如果代码执行到这里，说明没有活动的叠加层，可以更新和渲染主界面
    game_logic_.getStoryController().update();

    // 状态同步：根据Game状态切换Tab的显示，并动态更新内容
    GameState state = game_logic_.getCurrentState();
    auto requestOpt = game_logic_.getCurrentInputRequest();
    std::string inputPrompt = " 指令 ";

    switch(state) {
        case GameState::AwaitingTextInput:
            selectedInputMode_ = 1;
            if (requestOpt) inputPrompt = requestOpt->prompt;
            break;
        case GameState::AwaitingChoice:
            selectedInputMode_ = 2;
            if (requestOpt) {
                inputPrompt = requestOpt->prompt;
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
            inputPrompt = "正在播放剧情...";
            break;
        default: // GameState::InGame 或其他
            selectedInputMode_ = 0;
            break;
    }

    showSidePanels_ = (state == GameState::InGame);
    showPlayerStatus_ = (state == GameState::InGame);
    showFooter_ = (state != GameState::InStory);

    // -- 布局组装 --
    auto header = hbox({
        text("   拳王之路   ") | bold | color(Color::Red),
        filler(),
        text("当前位置: " + game_logic_.getPlayer().getLocation() + " ") | color(Color::Yellow),
    }) | border;

    Elements leftChildren;
    leftChildren.push_back(interactiveMainView_->Render() | vscroll_indicator | yframe | flex);
    if (showPlayerStatus_) {
        leftChildren.push_back(
            window(text(" 玩家状态 "),
                   hbox({
                       text("生命值: " + std::to_string(game_logic_.getPlayer().getHealth())) | color(Color::Green) | flex,
                       separator(),
                       text("疲劳值: " + std::to_string(game_logic_.getPlayer().getFatigue())) | color(Color::Yellow) | flex,
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

    auto mainContent = hbox({ leftPanel, rightPanel });
    Element mainLayout;

    if (state == GameState::InStory) {
        // 在故事模式下，我们构建一个不包含底部输入栏 (footer) 的布局。
        mainLayout = vbox({ header, mainContent | flex });
    } else {
        // 在其他模式下，正常创建并包含 footer。
        Element footer = window(text(inputPrompt), inputArea_->Render());
        mainLayout = vbox({ header, mainContent | flex, footer });
    }

    return mainLayout;
}

GameLayout::~GameLayout() = default;