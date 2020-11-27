#include "player.h"

Player::Player()
{
    x = 1;
    y = 6;
}

void Player::setX(int xCo)
{
   x = xCo;
}

int Player::getX()
{
   return x;
}

void Player::setY(int yCo)
{
    y = yCo;
}

int Player::getY()
{
    return y;
}

void Player::setKey(bool state)
{
    hasKey = state;
}

bool Player::keyEquipped()
{
    return hasKey;
}

void Player::setWeapon(bool state)
{
    hasWeapon = state;
}

bool Player::weaponEquipped()
{
    return hasWeapon;
}

// moves x coordinate to x + spaces
std::vector<std::tuple<int, int>> Player::moveRight(int spaces)
{
    // list of coordinates traversed by this move
    std::vector<std::tuple<int, int>> coordinateList;
    coordinateList.push_back(std::tuple<int, int>(getX(), getY())); // push on current position

    if(spaces <= 0) {
        while(x < 9)
        {
            x+=1;
            coordinateList.push_back(std::tuple<int, int>(x, y));
        }
        return coordinateList;
    }

    // check spaces so that the player wont move off the grid
    int walk;
    if((x + spaces) > 9)
        walk = 9-x;
    else
        walk = spaces;

    for (int i = 0; i < walk; i++)
    {
        x+=1;
        coordinateList.push_back(std::tuple<int, int>(x, y));
    }

    return coordinateList;
}

// moves x coordinate to x - spaces
std::vector<std::tuple<int, int>> Player::moveLeft(int spaces)
{
    // list of coordinates traversed by this move
    std::vector<std::tuple<int, int>> coordinateList;
    coordinateList.push_back(std::tuple<int, int>(getX(), getY())); // push on current position

    if(spaces <= 0) {
        while(x > 0)
        {
            x--;
            coordinateList.push_back(std::tuple<int, int>(x, y));
        }
        return coordinateList;
    }

    // check spaces so that the player wont move off the grid
    int walk;
    if((x - spaces) < 0)
        walk = x;
    else
        walk = spaces;

    for (int i = 0; i < walk; i++)
    {
        x--;
        coordinateList.push_back(std::tuple<int, int>(x, y));
    }

    return coordinateList;
}

std::vector<std::tuple<int, int>> Player::moveDown(int spaces)
{
    // list of coordinates traversed by this move
    std::vector<std::tuple<int, int>> coordinateList;
    coordinateList.push_back(std::tuple<int, int>(getX(), getY())); // push on current position

    if(spaces <= 0) {
        while(y < 9)
        {
            y+=1;
            coordinateList.push_back(std::tuple<int, int>(x, y));
        }
        return coordinateList;
    }

    // check spaces so that the player wont move off the grid
    int walk;
    if((y + spaces) > 9)
        walk = 9-y;
    else
        walk = spaces;

    for (int i = 0; i < walk; i++)
    {
        y+=1;
        coordinateList.push_back(std::tuple<int, int>(x, y));
    }

    return coordinateList;
}

std::vector<std::tuple<int, int>> Player::moveUp(int spaces)
{
    // list of coordinates traversed by this move
    std::vector<std::tuple<int, int>> coordinateList;
    coordinateList.push_back(std::tuple<int, int>(getX(), getY())); // push on current position

    if(spaces <= 0) {
        while(y > 0)
        {
            y-=1;
            coordinateList.push_back(std::tuple<int, int>(x, y));
        }
        return coordinateList;
    }

    // check spaces so that the player wont move off the grid
    int walk;
    if((y - spaces) < 0)
        walk = y;
    else
        walk = spaces;

    for (int i = 0; i < walk; i++)
    {
        y-=1;
        coordinateList.push_back(std::tuple<int, int>(x, y));
    }

    return coordinateList;
}


bool Player::hasAKey()
{
    return hasKey;
}

bool Player::canAttack()
{
    return hasWeapon;
}
