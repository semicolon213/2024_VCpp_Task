#pragma once

#include <chrono>

#include "resource.h"
#include "Struct.h"

/// ���� ������Ʈ ����
GameObject player;
std::vector<GameObject> enemy;
std::vector<GameObject> bullet;
std::vector<GameObject> obstacles;
std::vector<Item> items;

int score = 0;