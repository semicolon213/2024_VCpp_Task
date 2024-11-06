#pragma once
#include <vector>

#include "framework.h"

struct GameObject
{
	float x, y;
	SIZE objectSize;
	float speed;
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
	COUNTDONW,
	PLAY,
	GAMEOVER
};


