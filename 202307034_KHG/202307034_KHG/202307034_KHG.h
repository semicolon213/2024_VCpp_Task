#pragma once

#include <chrono>

#include "resource.h"
#include "Struct.h"

/// 게임 오브젝트 선언
GameObject player;
std::vector<GameObject> enemy;
std::vector<GameObject> bullet;
std::vector<GameObject> obstacles;
std::vector<Item> items;

int score = 0;