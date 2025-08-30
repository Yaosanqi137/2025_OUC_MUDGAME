#include "Player.h"

Player::Player() : x(-1), y(-1) {
}

void Player::setName(std::string name) {
    this->name = name;
}

int Player::getX() const {
    return x;
}

int Player::getY() const {
    return y;
}

const std::string &Player::getName() {
    return name;
}

void Player::setPos(int x, int y) {
    this->x = x;
    this->y = y;
}

void Player::move(int dx, int dy) {
    this->x += x;
    this->y += y;
}
