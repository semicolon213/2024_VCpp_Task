#pragma once
#include <vector>

#include "framework.h"

struct GameObject
{
	double x, y;
	SIZE objectSize;
	double speed;
	bool isActive;
	int health;
};

enum class ItemType
{
	DAMAGEUP,
	MULTISHOT,
	HEAL
};

struct Item
{
	float x, y;
	SIZE itemSize;
	bool isActive;
	ItemType type;
};

enum class GameState
{
	STARTSCREEN,
	COUNTDOWN,
	PLAY,
	GAMEOVER
};


