#pragma once
#include <vector>
#include <chrono>

enum class ItemType
{
    DAMAGEUP,
    FIRERATE,
    HEAL
};

enum class GameState
{
    STARTSCREEN,
    COUNTDOWN,
    PLAY,
    GAMEOVER
};

struct GameObject {
    double x, y;
    double speed;
    bool isActive;
    int health;
    double direction;
    std::chrono::steady_clock::time_point invincibleUntil;
    bool isInvincible;
};

struct Item {
    double x, y;
    bool isActive;
    ItemType type;
};
