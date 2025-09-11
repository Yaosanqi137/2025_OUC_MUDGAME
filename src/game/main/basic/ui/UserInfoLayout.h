#pragma once

#include "ftxui/component/component.hpp"
#include <memory>
#include <functional>

class Game;
class Player;

class UserInfoLayout : public ftxui::ComponentBase {
public:
    explicit UserInfoLayout(Game& game_logic);

    ftxui::Element Render() override;
    void show();
    void hide();
    [[nodiscard]] bool isShowing() const;

private:
    std::string formatGameTime() const;
    std::string formatPlayerStats() const;
    std::string formatMoney() const;

    Game& game_logic_;
    Player& player_;
    bool isShowing_ = false;

    // UI组件
    ftxui::Component exitButton_;
    ftxui::Component mainContainer_;
};
