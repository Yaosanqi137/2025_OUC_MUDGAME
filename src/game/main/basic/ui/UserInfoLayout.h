#ifndef USERINFOLAYOUT_H
#define USERINFOLAYOUT_H

#include "ftxui/component/component.hpp"
#include <memory>

class Game;
class Player;

class UserInfoLayout : public ftxui::ComponentBase {
public:
    explicit UserInfoLayout(Game& game_logic, bool& isShowingFlag);

    ftxui::Element OnRender() override;

private:
    std::string formatGameTime() const;
    std::string formatMoney() const;

    Game& game_logic_;
    Player& player_;
    bool& isShowingFlag_;

    // UI组件
    ftxui::Component exitButton_;
    ftxui::Component mainContainer_;
};

#endif // USERINFOLAYOUT_H