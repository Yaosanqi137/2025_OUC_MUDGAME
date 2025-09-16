#ifndef INC_2025_OUC_MUDGAME_PHONELAYOUT_H
#define INC_2025_OUC_MUDGAME_PHONELAYOUT_H

#include <ftxui/component/component_base.hpp>
#include <ftxui/component/component.hpp>
#include <memory>

class Game; // 前向声明
class UserInfoLayout; // 前向声明
class ShopLayout; // 前向声明

/**
 * @class PhoneLayout
 * @brief 实现了手机界面的FTXUI组件。
 * @details 包含应用图标和退出按钮，作为一个全屏覆盖层显示。
 */
class PhoneLayout : public ftxui::ComponentBase {
public:
    explicit PhoneLayout(Game& game_logic, bool& isShowingFlag,
                         std::function<void()> onMapClick,
                         std::function<void()> onShopClick,
                         std::function<void()> onInfoClick);

    ftxui::Element OnRender() override;

private:
    Game& game_logic_;
    bool& isShowingFlag_;

    // --- 子组件成员 ---
    ftxui::Component buttonMap_;
    ftxui::Component buttonShop_;
    ftxui::Component buttonInfo_;
    ftxui::Component buttonHome_;
    ftxui::Component mainContainer_;

    std::function<void()> onMapClick_; // 地图按钮
    std::function<void()> onShopClick_; // 网购平台按钮
    std::function<void()> onInfoClick_; // 用户信息按钮
};

#endif //INC_2025_OUC_MUDGAME_PHONELAYOUT_H