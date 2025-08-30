#ifndef PLAYER_H
#define PLAYER_H

#include <iostream>

class Player {
public:
    Player();

    const std::string& getName();
    void setName(std::string name);

    int getX() const;
    int getY() const;
    void setPos(int x, int y);
    void move(int dx, int dy);
private:
    std::string name;
    int x;
    int y;
};


#endif // PLAYER_H
