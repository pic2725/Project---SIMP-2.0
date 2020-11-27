#ifndef PLAYER_H
#define PLAYER_H

#include <vector>
#include <tuple>
#include <QString>

class Player
{
public:
    Player();

private:
    int x = 0;  // x position on map
    int y = 0;  // y position on map
    bool hasKey = false;
    bool hasWeapon = false;

public:
    void setX(int xCo);
    int getX();
    void setY(int yCo);
    int getY();
    void setKey(bool state);
    bool keyEquipped();
    void setWeapon(bool state);
    bool weaponEquipped();

    // move methods return a tuple-vector of coordinates traversed by move
    // the coordinates are like a 2-d array meaning y as 0 is the top row
    // and greater y coordinates are further down.
    std::vector<std::tuple<int, int>> moveRight(int spaces = 0);
    std::vector<std::tuple<int, int>> moveLeft(int spaces = 0);
    std::vector<std::tuple<int, int>> moveUp(int spaces = 0);
    std::vector<std::tuple<int, int>> moveDown(int spaces = 0);

    bool hasAKey();
    bool canAttack();

};

#endif // PLAYER_H
